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

#pragma once

#include <string>
#include <vector>

#include "sql/stmt/stmt.h"
#include "sql/stmt/select_stmt.h"

class Db;

typedef AttrInfoSqlNode CreateTableInfo;

/**
 * @brief 表示创建表的语句
 * @ingroup Statement
 * @details 虽然解析成了stmt，但是与原始的SQL解析后的数据也差不多
 */
class CreateTableStmt : public Stmt
{
public:
  CreateTableStmt(const std::string &table_name, const std::vector<AttrInfoSqlNode> &attr_infos)
        : table_name_(table_name),
          attr_infos_(attr_infos)
  {}
  virtual ~CreateTableStmt() {
    if (select_stmt_) {
      delete select_stmt_;
      select_stmt_ = nullptr;
    }
  }

  StmtType type() const override { return StmtType::CREATE_TABLE; }

  const std::string &table_name() const { return table_name_; }
  void add_attr_info(CreateTableInfo &info) {
    attr_infos_.emplace_back(std::move(info));
  }
  const std::vector<AttrInfoSqlNode> &attr_infos() const { return attr_infos_; }

  void set_select_stmt(SelectStmt *select_stmt) {
    select_stmt_ = select_stmt;
  }

  SelectStmt *select_stmt() {
    return select_stmt_;
  }

  void set_db(Db *db) {
    db_ = db;
  }

  Db *db() {
    return db_;
  }

  static RC create(Db *db, const CreateTableSqlNode &create_table, Stmt *&stmt);

public:
  bool create_view_ = false;

private:
  std::string table_name_;
  std::vector<AttrInfoSqlNode> attr_infos_;
  SelectStmt *select_stmt_ = nullptr;
  Db *db_ = nullptr;
};