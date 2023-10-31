/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/6/13.
//

#include "sql/executor/create_table_executor.h"

#include "session/session.h"
#include "common/log/log.h"
#include "storage/table/table.h"
#include "sql/stmt/create_table_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "event/sql_event.h"
#include "event/session_event.h"
#include "storage/db/db.h"
#include "sql/operator/create_table_select_physical_operator.h"
#include "sql/expr/expression.h"

RC CreateTableExecutor::execute(SQLStageEvent *sql_event)
{
  Stmt *stmt = sql_event->stmt();
  Session *session = sql_event->session_event()->session();
  ASSERT(stmt->type() == StmtType::CREATE_TABLE, 
         "create table executor can not run this command: %d", static_cast<int>(stmt->type()));

  CreateTableStmt *create_table_stmt = static_cast<CreateTableStmt *>(stmt);

  const int attribute_count = static_cast<int>(create_table_stmt->attr_infos().size());

  const char *table_name = create_table_stmt->table_name().c_str();
  RC rc = session->get_current_db()->create_table(table_name, attribute_count, create_table_stmt->attr_infos().data());

  return rc;
}

static void init_create_table_info(AttrType type, const std::string &&name, bool not_null, int len, CreateTableInfo &info) {
  info.type = type;
  info.name = std::move(name);
  info.not_null = not_null;
  info.length = len;
}

static Table *find_table(const char *table_name, const std::vector<Table *> &tables) {
  for (Table *table : tables) {
    if (strcmp(table->name(), table_name) == 0) {
      return table;
    }
  }
  return nullptr;
}

static RC handle_no_table(const TupleSchema &schema, CreateTableStmt *create_table_stmt) {
  Record record;
  std::vector<Value> values;
  for (int i = 0; i < schema.cell_num(); ++i) {
    const TupleCellSpec &spec = schema.cell_at(i);
    Expression *expr = spec.expression();
    if (!expr) {
      return RC::SQL_SYNTAX;
    }
    values.push_back(Value());
    RC rc = expr->try_get_value(values.back());
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }
  for (auto &val : values) {
    
  }
  return RC::SUCCESS;
}

RC CreateTableExecutor::execute(CreateTableSelectPhysicalOperator *oper) {
  CreateTableStmt *create_table_stmt = oper->create_table_stmt();
  SelectStmt *select_stmt = create_table_stmt->select_stmt();
  Db *db = oper->db();
  auto &project = oper->children()[0];
  assert(db != nullptr);
  assert(create_table_stmt != nullptr);
  RC rc = project->next();
  if (rc != RC::SUCCESS) {
    LOG_ERROR("find first record fail when create table.");
    return rc;
  }
  // 有可能没有from语句，要注意
  const std::vector<Table *> &tables = select_stmt->tables();
  bool no_tables = tables.empty();
  Tuple *tuple = project->current_tuple();
  const TupleSchema &schema = oper->schema();
  // 先做单表的试试看
  Table *table = nullptr;
  // TODO 把table初始化, 这个是原始的table
  for (int i = 0; i < schema.cell_num(); ++i) {
    const TupleCellSpec &spec = schema.cell_at(i);
    CreateTableInfo info;
    const Expression *expr = spec.expression();
    if (expr) {
      AttrType attr_type = expr->value_type();
      if (attr_type != UNDEFINED && attr_type != CHARS) {
        init_create_table_info(attr_type, spec.to_string(), false, 4, info);
        create_table_stmt->add_attr_info(info);
        continue;
      }
      if (no_tables && attr_type == CHARS) {
        Value tmp;
        expr->try_get_value(tmp);
        assert(tmp.attr_type() != AttrType::UNDEFINED);
        init_create_table_info(attr_type, spec.to_string(), false, tmp.get_string().size(), info);
      }
    }
    const char *table_name = spec.table_name();
    const char *field_name = spec.field_name();
    if (!table_name) {
      table = tables[0];
    }else {
      table = find_table(table_name, tables);
    }
    // 这里应该不用考虑投影子查询的情况
    // TODO 考虑聚合函数和函数的情况，考虑是算数表达式的情况
    if (field_name) {
      const TableMeta &table_meta = table->table_meta();
      const FieldMeta *field_meta = table_meta.field(field_name);
      init_create_table_info(field_meta->type(), spec.to_string(), field_meta->not_null(), field_meta->len(), info);   
    }
    create_table_stmt->add_attr_info(info);
  }
  const std::vector<CreateTableInfo> &infos = create_table_stmt->attr_infos();
  RC rc = db->create_table(create_table_stmt->table_name().c_str(), infos.size(), infos.data());
  if (rc != RC::SUCCESS) {
    return rc;
  }

  // const TableMeta &table_meta = table->table_meta();
  // const FieldMeta *field_meta = table_meta.field(0);
  // char *data = (char *)malloc(table_meta.record_size());
  // record.set_data_owner(data, table_meta.record_size(), field_meta->offset());

  // 往表中插入数据
  // table->insert_record();
  // while (true);
  const char *table_name = create_table_stmt->table_name().c_str();
  return RC::INTERNAL;
}