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

static inline void init_create_table_info(AttrType type, const std::string &&name, bool not_null, int len, CreateTableInfo &info) {
  info.type = type;
  info.name = std::move(name);
  info.not_null = not_null;
  info.length = len;
}

/**
 * 本方法的values中的元素个数必须与table的field值匹配
 */
static RC insert_record_to_table(Table *table, int size, int bitmap_len, std::vector<Value> &values) {
  Record record;
  char *data = (char *)malloc(size);
  record.set_data_owner(data, size, bitmap_len);
  memset(data, 0, bitmap_len);
  common::Bitmap bitmap(data, bitmap_len);
  data += bitmap_len;
  for (int i = 0; i < values.size(); ++i) {
    Value &val = values[i];
    if (val.is_null()) {
      bitmap.clear_bit(i);
      continue;
    }
    const char *val_data = val.data();
    memcpy(data, val_data, val.length());
    data += val.length();
  }
  return table->insert_record(record);
}

static Table *find_table(const char *table_name, const std::vector<Table *> &tables) {
  for (Table *table : tables) {
    if (strcmp(table->name(), table_name) == 0) {
      return table;
    }
  }
  return nullptr;
}

static RC handle_create_without_table(const TupleSchema &schema, CreateTableStmt *create_table_stmt, Db *db) {
  Record record;
  std::vector<Value> values;
  values.resize(schema.cell_num());
  ValueListTuple tuple;
  for (int i = 0; i < schema.cell_num(); ++i) {
    const TupleCellSpec &spec = schema.cell_at(i);
    Expression *expr = spec.expression();
    if (!expr) {
      return RC::SQL_SYNTAX;
    }
    RC rc = expr->get_value(tuple, values[i]);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    CreateTableInfo info;
    const Value &value = values[i];
    // 由于没有表的查询，所以不清楚能不能插入NULL值
    init_create_table_info(value.attr_type(), spec.to_string(), false, value.length(), info);
    // 要检查一下这样写会不会有问题，内部虽然用的移动语义，但是我不太会用
    create_table_stmt->add_attr_info(info);
  }
  const std::vector<CreateTableInfo> &infos = create_table_stmt->attr_infos();
  RC rc = db->create_table(create_table_stmt->table_name().c_str(), infos.size(), infos.data());
  if (rc != RC::SUCCESS) {
    LOG_WARN("table: %s create fail, rc=%s", create_table_stmt->table_name().c_str(), strrc(rc));
    return rc;
  }
  Table *table = db->find_table(create_table_stmt->table_name().c_str());
  if (table == nullptr) {
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  
  int bitmap_len = common::bitmap_size(schema.cell_num());
  int size = table->table_meta().record_size();
  rc = insert_record_to_table(table, size, bitmap_len, values);
  if (rc != RC::SUCCESS) {
    db->drop_table(table->name());
    return RC::INTERNAL;
  }
  return RC::RECORD_EOF;
}

static RC handle_normal_field(const TupleCellSpec &spec, CreateTableInfo &info, const std::vector<Table *> &tables) {
  Table *table;
  if (tables.size() == 1) {
    table = tables[0];
  }else {
    table = find_table(spec.table_name(), tables);
    if (table == nullptr) {
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
  }
  const TableMeta table_meta = table->table_meta();
  const FieldMeta *field_meta = table_meta.field(spec.field_name());
  if (field_meta == nullptr) {
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }
  init_create_table_info(field_meta->type(), spec.to_string(), field_meta->not_null(), field_meta->len(), info);
  return RC::SUCCESS;
}

RC CreateTableExecutor::execute(CreateTableSelectPhysicalOperator *oper) {
  CreateTableStmt *create_table_stmt = oper->create_table_stmt();
  SelectStmt *select_stmt = create_table_stmt->select_stmt();
  Db *db = oper->db();
  assert(db != nullptr);
  assert(create_table_stmt != nullptr);
  RC rc = RC::SUCCESS;
  const TupleSchema &schema = oper->schema();
  if (rc != RC::SUCCESS) {
    LOG_ERROR("find first record fail when create table.");
    return rc;
  }

  const std::vector<Table *> &tables = select_stmt->tables();
  // 处理没有from语句的情况，比如select FUNC('1111');
  if (tables.empty()) {
    rc = handle_create_without_table(schema, create_table_stmt, db);
    return rc;
  }
  auto &project = oper->children()[0];
  rc = project->next();
  if (rc != RC::SUCCESS) {
    return rc;
  }
  Tuple *tuple = project->current_tuple();
  std::vector<Value> values;
  values.resize(schema.cell_num());
  // 先做单表的试试看
  Table *table = nullptr;
  for (int i = 0; i < schema.cell_num(); ++i) {
    const TupleCellSpec &spec = schema.cell_at(i);
    CreateTableInfo info;
    const Expression *expr = spec.expression();
    tuple->cell_at(i, values[i]);
    if (expr) {
      AttrType attr_type = expr->value_type();
      if (attr_type == UNDEFINED || attr_type == BOOLEANS || attr_type == NULLS) {
        LOG_ERROR("other type unimplenment");
        return RC::INTERNAL;
      }
      if (attr_type != CHARS) {
        init_create_table_info(attr_type, spec.to_string(), false, 4, info);
        create_table_stmt->add_attr_info(info);
        continue;
      }
      switch (expr->type()) {
        case ExprType::FUNC: {
          const FuncExpr *func_expr = static_cast<const FuncExpr *>(expr);
          if (func_expr->func_type() != FUNC_DATE_FORMAT) {
            LOG_ERROR("this func type unimplenment create table select");
            return RC::INTERNAL;
          }
          Value &value = values[i];
          init_create_table_info(CHARS, spec.to_string(), false, 74, info);
          create_table_stmt->add_attr_info(info);
        }break;
        default: {
          LOG_ERROR("this expr type unimplenment");
          return RC::INTERNAL;
        }
      }
      continue;
    }
    AggFuncType aggr_func = spec.aggfunc_type();
    if (aggr_func != FUNC_NONE) {
      const char *table_name = spec.table_name();
      const char *field_name = spec.field_name();
      if (!field_name) {
        ValueExpr *value_expr = new ValueExpr;
        AggrFuncExpr expr(aggr_func, value_expr);
        init_create_table_info(expr.value_type(), spec.to_string(), false, values[i].length(), info);
        create_table_stmt->add_attr_info(info);
        continue;
      }
      Table *table = nullptr;
      if (tables.size() == 1) {
        table = tables[0];
      }else {
        table = find_table(table_name, tables);
      }
      FieldExpr *field_expr = new FieldExpr(table, table->table_meta().field(field_name), aggr_func);
      AggrFuncExpr expr(aggr_func, field_expr);
      init_create_table_info(expr.value_type(), spec.to_string(), false, values[i].length(), info);
      create_table_stmt->add_attr_info(info);
      continue;
    }
    // 普通字段
    handle_normal_field(spec, info, tables);
    create_table_stmt->add_attr_info(info);
  }
  const std::vector<CreateTableInfo> &infos = create_table_stmt->attr_infos();
  rc = db->create_table(create_table_stmt->table_name().c_str(), infos.size(), infos.data());
  if (rc != RC::SUCCESS) {
    return rc;
  }

  table = db->find_table(create_table_stmt->table_name().c_str());
  if (table == nullptr) {
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  int bitmap_len = common::bitmap_size(schema.cell_num());
  int size = table->table_meta().record_size();
  // 第一次往表中插入数据
  rc = insert_record_to_table(table, size, bitmap_len, values);
  if (rc != RC::SUCCESS) {
    db->drop_table(table->name());
    return rc;
  }
  // 第n次向表中插入数据
  while (RC::SUCCESS == (rc = project->next())) {
    tuple = project->current_tuple();
    for (int i = 0; i < schema.cell_num(); ++i) {
      RC rc = tuple->cell_at(i, values[i]);
      if (rc != RC::SUCCESS) {
        // 测试
        assert(false);
      }
    }
    rc = insert_record_to_table(table, size, bitmap_len, values);
    if (rc != RC::SUCCESS) {
      db->drop_table(table->name());
      return rc;
    }
  }

  return rc == RC::RECORD_EOF? rc: RC::INTERNAL;
}