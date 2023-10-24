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
// Created by WangYunlai on 2022/6/7.
//

#pragma once

#include <iostream>
#include "storage/table/table.h"
#include "storage/field/field_meta.h"
#include "common/lang/string.h"

class TupleCellSpec
{
public:
  TupleCellSpec(const char *table_name, const char *field_name, const char *alias, AggFuncType type = AggFuncType::FUNC_NONE);
  TupleCellSpec(const char *alias, AggFuncType type = AggFuncType::FUNC_NONE);
  TupleCellSpec(const char *field_name, const char *alias, AggFuncType type = AggFuncType::FUNC_NONE);
  TupleCellSpec(FuncType func_type) {
    aggfunc_type_ = FUNC_NONE;
    func_type_ = func_type;
  }

  const char *table_name() const
  {
    return table_name_.c_str();
  }
  const char *field_name() const
  {
    return field_name_.c_str();
  }
  std::shared_ptr<std::string> alias() const
  {
    return alias_;
  }
  const AggFuncType aggfunc_type() const {
    return aggfunc_type_;
  }
  /**
   * 统一tuple_cell类的输出
   */
  const std::string to_string() const {
    if (alias_) {
      return *alias_;
    }
    std::string str;
    if (aggfunc_type_ != FUNC_NONE) {
      str.append(AGGR_FUNC_TYPE_NAME[aggfunc_type_]).append("(");
    }
    if (!common::is_blank(table_name_.c_str())) {
      str.append(table_name_).append(".");
    }
    str.append(field_name_);
    if (aggfunc_type_ != FUNC_NONE) {
      str.append(")");
    }
    return str;
  }

  void set_expr(Expression *expr) {
    expression_ = expr;
  }

  Expression *expression() {
    return expression_;
  }

private:
  AggFuncType aggfunc_type_;
  FuncType func_type_;
  std::string table_name_;
  std::string field_name_;
  Expression *expression_ = nullptr;
  std::shared_ptr<std::string> alias_ = nullptr;
};
