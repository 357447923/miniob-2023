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
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "common/log/log.h"
#include "common/typecast.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

RC checkAndCastValue(Value &value,const FieldMeta *field_meta) {
  const AttrType value_type = value.attr_type();
  if (value_type == NULLS) {
    if (field_meta->not_null()) {
      LOG_ERROR("You can't put NULL to an not null field");
      return RC::SCHEMA_FIELD_NOT_NULL;
    }
  } else {
    const AttrType field_type = field_meta->type();
    if (field_type != value_type) {
      // 对update.value进行数据类型转换
      RC rc = common::type_cast(value, field_meta->type());
      if (rc != RC::SUCCESS) {
        LOG_WARN("field type mismatch. field=%s, field_type=%d, value_type=%d",
            field_meta->name(), field_type, value_type);
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
    }
  }
  return RC::SUCCESS;  
}

UpdateStmt::UpdateStmt(Table *table,  
                       int value_amount, 
                       FilterStmt *filter_stmt,  
                       std::unordered_map <std::string, Value*> &update_map)
    : table_(table), value_amount_(value_amount), filter_stmt_(filter_stmt)
{
  update_map_.swap(update_map);
}

RC UpdateStmt::create(Db *db, UpdateSqlNode &update, Stmt *&stmt)
{
  // 检查db和表名合法性
  const char *table_name = update.relation_name.c_str();
  if (nullptr == db || nullptr == table_name) {
    LOG_WARN("invalid argument. db=%p, table_name=%p",
        db, table_name);
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  // 检查更新的字段合法性
  const TableMeta &table_meta = table->table_meta();
  std::vector<SetClauseSqlNode> &set_clause_list= update.set_clause_list;
  std::unordered_map <std::string, Value*> update_map;
  std:: unordered_map <std::string, Value> update_map_test;
  for (SetClauseSqlNode &setClause : set_clause_list)
  {
      const FieldMeta *field_meta = table_meta.field(setClause.attribute_name_.c_str());
      if (nullptr == field_meta) {
        LOG_WARN("no such field. table_name=%s, field=%s", table_name, setClause.attribute_name_.c_str());
        return RC::SCHEMA_FIELD_NOT_EXIST;
      } 
      Value &tempValue = setClause.value_;
      RC rc = checkAndCastValue(tempValue,field_meta);
      if (rc!=RC::SUCCESS)
      {
        return rc;
      }
      update_map[setClause.attribute_name_.c_str()] = &(setClause.value_);
      std::cout << update_map[setClause.attribute_name_] << std::endl;
  }
  std::unordered_map<std::string, Table *> table_map;
  table_map.insert(std::pair<std::string, Table *>(update.relation_name, table));
  std::vector<Table *> tables;
  tables.push_back(table);
  // 构造过滤语句
  FilterStmt *filter_stmt = nullptr;
  RC rc = FilterStmt::create(
    db, table, &table_map, tables, update.conditions.data(), static_cast<int>(update.conditions.size()), filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create filter statement. rc=%d:%s", rc, strrc(rc));
    return rc;
  }
  stmt = new UpdateStmt(table, set_clause_list.size(), filter_stmt, update_map);
  return RC::SUCCESS;
}

