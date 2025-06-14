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

#pragma once

#include <vector>
#include <unordered_map>
#include "sql/parser/parse_defs.h"
#include "sql/stmt/stmt.h"
#include "sql/expr/expression.h"

class Db;
class Table;
class FieldMeta;

struct FilterObj 
{
  bool is_attr;
  Expression *expression = nullptr;
  Value value;

// 由于expression在目前最后在logical_plan_generator阶段交给智能指针管理
// 故此处不实现对应的析构函数，避免多次释放

  void init_attr(Expression *expression) {
    is_attr = true;
    this->expression = expression;
  }

  void init_attr(Field &&field) {
    is_attr = true;
    expression = new FieldExpr(std::move(field));
  }

  void init_value(const Value &value)
  {
    is_attr = false;
    this->value = value;
  }
};

class FilterUnit 
{
public:
  FilterUnit() = default;
  ~FilterUnit()
  {
    if (left_ != nullptr) {
      delete left_;
      left_ = nullptr;
    }
    if (right_ != nullptr) {
      delete right_;
      right_ = nullptr;
    }
  }

  void set_comp(CompOp comp)
  {
    comp_ = comp;
  }

  CompOp comp() const
  {
    return comp_;
  }

  void set_left(const FilterObj *obj)
  {
    left_ = obj;
  }
  void set_right(const FilterObj *obj)
  {
    right_ = obj;
  }

  const FilterObj &left() const
  {
    return *left_;
  }
  const FilterObj &right() const
  {
    return *right_;
  }

private:
  CompOp comp_ = NO_OP;
  const FilterObj *left_ = nullptr;
  const FilterObj *right_ = nullptr;
};

/**
 * @brief Filter/谓词/过滤语句
 * @ingroup Statement
 */
class FilterStmt 
{
public:
  FilterStmt() = default;
  virtual ~FilterStmt();

public:
  const std::vector<FilterUnit *> &filter_units() const
  {
    return filter_units_;
  }
  ConjunctionExpr::Type conjuct_type() const {
    return conjuct_type_;
  }

public:
  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables_map,
      const std::vector<Table *> &tables, const ConditionSqlNode *conditions, int condition_num, FilterStmt *&stmt);

  static RC create_filter_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables_map,
      const std::vector<Table *> &tables, const ConditionSqlNode &condition, FilterUnit *&filter_unit);

private:
  std::vector<FilterUnit *> filter_units_;  // 默认当前都是AND关系
  ConjunctionExpr::Type conjuct_type_ = ConjunctionExpr::Type::AND;
};
