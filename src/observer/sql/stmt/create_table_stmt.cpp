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

#include "sql/stmt/create_table_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "event/sql_debug.h"

RC CreateTableStmt::create(Db *db, const CreateTableSqlNode &create_table, Stmt *&stmt)
{
  CreateTableStmt *create_table_stmt = new CreateTableStmt(create_table.relation_name, create_table.attr_infos);
  if (create_table.select_table != nullptr) {
    std::unordered_map<std::string, Table *> parent_table_map;
    std::vector<Table *> parent_tables;
    Stmt *select_stmt = nullptr;
    RC rc = SelectStmt::create(db, *create_table.select_table, parent_tables, parent_table_map, select_stmt);
    if (rc != RC::SUCCESS) {
      sql_debug("create table statement fail: select_stmt create fail");
      delete create_table_stmt;
      return rc;
    }
    std::vector<AttrInfoSqlNode> &attr_infos = create_table_stmt->attr_infos_;
    if (!attr_infos.empty()) {
      SelectStmt *select = static_cast<SelectStmt *>(select_stmt);
      if (select->query_expressions().size() != attr_infos.size()) {
        return RC::SQL_SYNTAX;
      }
    }
    create_table_stmt->select_stmt_ = static_cast<SelectStmt *>(select_stmt);
    create_table_stmt->db_ = db;
  }
  stmt = create_table_stmt;
  sql_debug("create table statement: table name %s", create_table.relation_name.c_str());
  return RC::SUCCESS;
}
