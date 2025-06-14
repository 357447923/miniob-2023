/* Copyright (c) 2021OceanBase and/or its affiliates. All rights reserved.
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

#include "sql/stmt/insert_stmt.h"
#include "common/log/log.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "common/date.h"
#include "common/typecast.h"
#include "event/sql_debug.h"
InsertStmt::InsertStmt(Table *table, const std::vector<std::vector<Value>> *values, int value_amount)
    : table_(table), values_(values), value_amount_(value_amount)
{}

RC InsertStmt::create(Db *db, const InsertSqlNode &inserts, Stmt *&stmt)
{
  const char *table_name = inserts.relation_name.c_str();
  const std::vector<std::vector<Value>> &value_vector = inserts.values;
  if (nullptr == db || nullptr == table_name || value_vector.empty() || value_vector[0].empty()) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, value_num=%d",
        db, table_name, static_cast<int>(value_vector[0].size()));
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  // check the fields number
  const int value_num = static_cast<int>(value_vector[0].size());
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num() - table_meta.sys_field_num();
  if (field_num != value_num) {
    LOG_WARN("schema mismatch. value num=%d, field num in schema=%d", value_num, field_num);
    return RC::SCHEMA_FIELD_MISSING;
  }

  // check fields type
  for (auto &values : value_vector) {
    const int sys_field_num = table_meta.sys_field_num();
    // 在词法语法解析中，我只把std::vector<Value>进行reverse
    for (int i = 0; i < value_num; i++) {
      const FieldMeta *field_meta = table_meta.field(i + sys_field_num);
      const AttrType value_type = values[i].attr_type();
      if (value_type == NULLS) {
        if (field_meta->not_null()) {
          LOG_ERROR("Unacceptable operation: put NULL into a NOT NULL field");
          return RC::SCHEMA_FIELD_NOT_NULL;
        }
        continue;
      }
      const AttrType field_type = field_meta->type();
      if (field_type != value_type) {
        Value& value = const_cast<Value&> (values[i]);
        RC rc = common::type_cast(value, field_type);
        if (rc == RC::SUCCESS) {
          continue;
        }
        LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d",
            table_name, field_meta->name(), field_type, value_type);
        return RC::SCHEMA_FIELD_TYPE_MISMATCH;
      }
      int field_max_len = field_meta->len();
      if (values[i].length() > field_max_len) {
        return RC::SQL_SYNTAX;
      }
    }
  }
  
  // everything alright
  InsertStmt * insert_stmt = new InsertStmt(table, &value_vector, value_vector.size());
  stmt = insert_stmt;
  return RC::SUCCESS;
}
