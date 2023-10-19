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
// Created by Wangyunlai on 2022/07/05.
//

#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/parser/parse_defs.h"

using namespace std;

RC FieldExpr::get_value(const Tuple &tuple, Value &value) const
{
  return tuple.find_cell(TupleCellSpec(table_name(), field_name(), nullptr, AggFuncType::FUNC_NONE), value);
}

RC ValueExpr::get_value(const Tuple &tuple, Value &value) const
{
  value = value_;
  return RC::SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
CastExpr::CastExpr(unique_ptr<Expression> child, AttrType cast_type)
    : child_(std::move(child)), cast_type_(cast_type)
{}

CastExpr::~CastExpr()
{}

RC CastExpr::cast(const Value &value, Value &cast_value) const
{
  RC rc = RC::SUCCESS;
  if (this->value_type() == value.attr_type()) {
    cast_value = value;
    return rc;
  }

  switch (cast_type_) {
    case BOOLEANS: {
      bool val = value.get_boolean();
      cast_value.set_boolean(val);
    } break;
    default: {
      rc = RC::INTERNAL;
      LOG_WARN("unsupported convert from type %d to %d", child_->value_type(), cast_type_);
    }
  }
  return rc;
}

RC CastExpr::get_value(const Tuple &tuple, Value &cell) const
{
  RC rc = child_->get_value(tuple, cell);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(cell, cell);
}

RC CastExpr::try_get_value(Value &value) const
{
  RC rc = child_->try_get_value(value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, value);
}

////////////////////////////////////////////////////////////////////////////////

ComparisonExpr::ComparisonExpr(CompOp comp, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : comp_(comp), left_(std::move(left)), right_(std::move(right))
{}

ComparisonExpr::~ComparisonExpr()
{}

RC ComparisonExpr::compare_value(const Value &left, const Value &right, bool &result) const
{
  RC rc = RC::SUCCESS;
  RC cmp_result = RC::LEFT_CAN_NOT_CMP_TO_ANOTHER;
  if (comp_ == LIKE_OP || comp_ == NOT_LIKE_OP)
  {
    cmp_result = left.like_compare(right);
  } else{
    cmp_result = left.compare(right);
  }
   
  result = false;
  const bool comp_can_cmp = cmp_result != RC::LEFT_CAN_NOT_CMP_TO_ANOTHER;

  switch (comp_) {
    case EQUAL_TO: {
      result = comp_can_cmp && (RC::LEFT_EQ_ANOTHER == cmp_result);
    } break;
    case LESS_EQUAL: {
      result = comp_can_cmp && (cmp_result == RC::LEFT_LT_ANOTHER || cmp_result == RC::LEFT_EQ_ANOTHER);
    } break;
    case NOT_EQUAL: {
      result = comp_can_cmp && (cmp_result != RC::LEFT_EQ_ANOTHER);
    } break;
    case LESS_THAN: {
      result = comp_can_cmp && (cmp_result == RC::LEFT_LT_ANOTHER);
    } break;
    case GREAT_EQUAL: {
      result = comp_can_cmp && (cmp_result == RC::LEFT_GT_ANOTHER || cmp_result == RC::LEFT_EQ_ANOTHER);
    } break;
    case GREAT_THAN: {
      result = comp_can_cmp && (cmp_result == RC::LEFT_GT_ANOTHER);
    } break;
    case IS_NULL: {
      AttrType left_type = left.attr_type();
      AttrType right_type = right.attr_type();
      result = left_type == right_type && left_type == NULLS;
    }break;
    case NOT_NULL: {
      AttrType left_type = left.attr_type();
      AttrType right_type = right.attr_type();
      result = (left_type == NULLS || right_type == NULLS) && (left_type != NULLS || right_type != NULLS);
    }break;
    case LIKE_OP : {
      AttrType left_type = left.attr_type();
      AttrType right_type = right.attr_type();
      result = (left_type == CHARS || right_type == CHARS) && (cmp_result == RC::LEFT_LIKE_ANOTHER);
    }break;
    case NOT_LIKE_OP : {
      AttrType left_type = left.attr_type();
      AttrType right_type = right.attr_type();
      result = (left_type == CHARS || right_type == CHARS) && (cmp_result == RC::LEFT_NOT_LIKE_ANOTHER);
    }break;
    default: {
      LOG_WARN("unsupported comparison. %d", comp_);
      rc = RC::INTERNAL;
    } break;
  }

  return rc;
}

RC ComparisonExpr::try_get_value(Value &cell) const
{
  if (left_->type() == ExprType::VALUE && right_->type() == ExprType::VALUE) {
    ValueExpr *left_value_expr = static_cast<ValueExpr *>(left_.get());
    ValueExpr *right_value_expr = static_cast<ValueExpr *>(right_.get());
    const Value &left_cell = left_value_expr->get_value();
    const Value &right_cell = right_value_expr->get_value();

    bool value = false;
    RC rc = compare_value(left_cell, right_cell, value);
    if (rc != RC::SUCCESS) {
      LOG_DEBUG("failed to compare tuple cells. rc=%s", strrc(rc));
    } else {
      cell.set_boolean(value);
    }
    return rc;
  }

  return RC::INVALID_ARGUMENT;
}

RC ComparisonExpr::get_value(const Tuple &tuple, Value &value) const
{
  Value left_value;
  Value right_value;

  RC rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  rc = right_->get_value(tuple, right_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }

  bool bool_value = false;
  rc = compare_value(left_value, right_value, bool_value);
  if (rc == RC::SUCCESS) {
    value.set_boolean(bool_value);
  }
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
ConjunctionExpr::ConjunctionExpr(Type type, vector<unique_ptr<Expression>> &children)
    : conjunction_type_(type), children_(std::move(children))
{}

RC ConjunctionExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    value.set_boolean(true);
    return rc;
  }

  Value tmp_value;
  for (const unique_ptr<Expression> &expr : children_) {
    rc = expr->get_value(tuple, tmp_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value by child expression. rc=%s", strrc(rc));
      return rc;
    }
    bool bool_value = tmp_value.get_boolean();
    if ((conjunction_type_ == Type::AND && !bool_value) || (conjunction_type_ == Type::OR && bool_value)) {
      value.set_boolean(bool_value);
      return rc;
    }
  }

  bool default_value = (conjunction_type_ == Type::AND);
  value.set_boolean(default_value);
  return rc;
}

////////////////////////////////////////////////////////////////////////////////

ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, Expression *left, Expression *right)
    : arithmetic_type_(type), left_(left), right_(right)
{}
ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : arithmetic_type_(type), left_(std::move(left)), right_(std::move(right))
{}

AttrType ArithmeticExpr::value_type() const
{
  if (!right_) {
    return left_->value_type();
  }

  if (left_->value_type() == AttrType::INTS &&
      right_->value_type() == AttrType::INTS &&
      arithmetic_type_ != Type::DIV) {
    return AttrType::INTS;
  }
  
  return AttrType::FLOATS;
}

RC ArithmeticExpr::calc_value(const Value &left_value, const Value &right_value, Value &value) const
{
  RC rc = RC::SUCCESS;

  const AttrType target_type = value_type();

  switch (arithmetic_type_) {
    case Type::ADD: {
      if (target_type == AttrType::INTS) {
        value.set_int(left_value.get_int() + right_value.get_int());
      } else {
        value.set_float(left_value.get_float() + right_value.get_float());
      }
    } break;

    case Type::SUB: {
      if (target_type == AttrType::INTS) {
        value.set_int(left_value.get_int() - right_value.get_int());
      } else {
        value.set_float(left_value.get_float() - right_value.get_float());
      }
    } break;

    case Type::MUL: {
      if (target_type == AttrType::INTS) {
        value.set_int(left_value.get_int() * right_value.get_int());
      } else {
        value.set_float(left_value.get_float() * right_value.get_float());
      }
    } break;

    case Type::DIV: {
      if (target_type == AttrType::INTS) {
        if (right_value.get_int() == 0) {
          // NOTE: 设置为整数最大值是不正确的。通常的做法是设置为NULL，但是当前的miniob没有NULL概念，所以这里设置为整数最大值。
          // 以上是原注释，现在的miniob已经有NULL概念了
          value.set_null();
        } else {
          value.set_int(left_value.get_int() / right_value.get_int());
        }
      } else {
        if (right_value.get_float() > -EPSILON && right_value.get_float() < EPSILON) {
          // NOTE: 设置为浮点数最大值是不正确的。通常的做法是设置为NULL，但是当前的miniob没有NULL概念，所以这里设置为浮点数最大值。
          // 以上是原注释，现在的miniob已经有NULL概念了
          value.set_null();
        } else {
          value.set_float(left_value.get_float() / right_value.get_float());
        }
      }
    } break;

    case Type::NEGATIVE: {
      if (target_type == AttrType::INTS) {
        value.set_int(-left_value.get_int());
      } else {
        value.set_float(-left_value.get_float());
      }
    } break;

    default: {
      rc = RC::INTERNAL;
      LOG_WARN("unsupported arithmetic type. %d", arithmetic_type_);
    } break;
  }
  return rc;
}

RC ArithmeticExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }else if (left_value.attr_type() == NULLS) {
    value.set_null();
    return rc;
  }
  if (this->arithmetic_type_ == Type::NEGATIVE) {
    return calc_value(left_value, right_value, value);
  }
  rc = right_->get_value(tuple, right_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }else if (right_value.attr_type() == NULLS) {
    value.set_null();
    return rc;
  }
  return calc_value(left_value, right_value, value);
}

RC ArithmeticExpr::try_get_value(Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->try_get_value(left_value);
  if (rc != RC::SUCCESS) {
    LOG_DEBUG("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  if (right_) {
    rc = right_->try_get_value(right_value);
    if (rc != RC::SUCCESS) {
      LOG_DEBUG("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  return calc_value(left_value, right_value, value);
}

void ArithmeticExpr::find_field_need(const Table *table, ArithmeticExpr *expression) {
  // 处理左边
  std::unique_ptr<Expression> &left = expression->left_;
  ExprType type = left->type();
  if (type == ExprType::ARITHMETIC) {
    // 如果还是算式，则递归执行
    find_field_need(table, (ArithmeticExpr *)left.get());
  } else if (type == ExprType::FIELD) {
    // 当前仅有类型为Field的expression需要填充
    FieldExpr *field_expr = (FieldExpr *)(left.get());
    const FieldMeta *field_meta = table->table_meta().field(field_expr->name().c_str());
    Field &field = field_expr->field();
    field.set_field(field_meta);
    field.set_table(table);
  }else if (type == ExprType::AGGRFUNCTION) {
    expression->contains_aggr_ = true;
    FieldExpr *field_expr = ((AggrFuncExpr *)left.get())->field();
    if (!field_expr) {
      return;
    }
    const FieldMeta *field_meta = table->table_meta().field(field_expr->name().c_str());
    Field &field = field_expr->field();
    field.set_field(field_meta);
    field.set_table(table);
  }
  if (expression->arithmetic_type_ == Type::NEGATIVE) {
    // 负值表达式是单目的运算
    return;
  }
  // 处理右边, 逻辑与左边的相同
  std::unique_ptr<Expression> &right = expression->right_;
  type = right->type();
  if (type == ExprType::ARITHMETIC) {
    find_field_need(table, (ArithmeticExpr *)right.get());
  }else if(type == ExprType::FIELD) {
    FieldExpr *field_expr = (FieldExpr *)(right.get());
    const FieldMeta *field_meta = table->table_meta().field(field_expr->name().c_str());
    Field &field = field_expr->field();
    field.set_field(field_meta);
    field.set_table(table);
  }else if (type == ExprType::AGGRFUNCTION) {
    FieldExpr *field_expr = ((AggrFuncExpr *)right.get())->field();
    expression->contains_aggr_ = true;
    if (!field_expr) {
      return;
    }
    const FieldMeta *field_meta = table->table_meta().field(field_expr->name().c_str());
    Field &field = field_expr->field();
    field.set_field(field_meta);
    field.set_table(table);
  }
  return;
}

static bool create_field_by_expr(FieldExpr *field_expr, const std::unordered_map<std::string, Table *> &table_map) {
  std::string field_name = field_expr->name();
  int idx = field_name.find_first_of(".");
  if (std::string::npos == idx) {
    LOG_ERROR("ERROR: idx=npos");
  }
  std::string table_name = field_name.substr(0, idx);
  auto iter = table_map.find(table_name);
  if (iter == table_map.end()) {
    LOG_WARN("no such table in from list: %s", table_name);
    return false;
  }
  Table *table = iter->second;
  field_name = field_name.substr(idx + 1, field_name.size() - idx - 1);
  const FieldMeta *field_meta = table->table_meta().field(field_name.c_str());
  Field &field = field_expr->field();
  field.set_field(field_meta);
  field.set_table(table);
  return true;
}

void ArithmeticExpr::find_field_need(const std::unordered_map<std::string, Table *> &table_map, ArithmeticExpr *expression) {
  // 处理左边
  std::unique_ptr<Expression> &left = expression->left_;
  ExprType type = left->type();
  if (type == ExprType::ARITHMETIC) {
    // 如果还是算式，则递归执行
    find_field_need(table_map, (ArithmeticExpr *)left.get());
  } else if (type == ExprType::FIELD) {
    // 当前仅有类型为Field的expression需要填充
    if (!create_field_by_expr((FieldExpr *)(left.get()), table_map)) {
      return;
    }
  } else if (type == ExprType::AGGRFUNCTION) {
    AggrFuncExpr *aggr_func_expr = (AggrFuncExpr*)(left.get());
    expression->contains_aggr_ = true;
    FieldExpr *field_expr = aggr_func_expr->field();
    // 补充算数表达式中的聚合函数表达式的field
    if (!field_expr || !create_field_by_expr(field_expr, table_map)) {
      return;
    }
  }
  if (expression->arithmetic_type_ == Type::NEGATIVE) {
    // 负值表达式是单目的运算
    return;
  }
  // 处理右边, 逻辑与左边的相同
  std::unique_ptr<Expression> &right = expression->right_;
  type = right->type();
  if (type == ExprType::ARITHMETIC) {
    find_field_need(table_map, (ArithmeticExpr *)right.get());
  }else if(type == ExprType::FIELD) {
     // 当前仅有类型为Field的expression需要填充
    if (!create_field_by_expr((FieldExpr *)(right.get()), table_map)) {
      return;
    }
  }else if(type == ExprType::AGGRFUNCTION) {
    FieldExpr *field_expr = ((AggrFuncExpr*)(right.get()))->field();
    expression->contains_aggr_ = true;
    if (!field_expr || !create_field_by_expr(field_expr, table_map)) {
      return;
    }
  }
  return;
}

void ArithmeticExpr::find_aggr_func(ArithmeticExpr *expression, std::vector<AggrFuncExpr *> &exprs) {
  Expression *left = expression->left_.get();
  switch (left->type()) {
    case ExprType::ARITHMETIC: {
      find_aggr_func((ArithmeticExpr *)left, exprs);
    }break;
    case ExprType::AGGRFUNCTION: {
      exprs.push_back((AggrFuncExpr *)left);
    }break;
  }
  if (expression->arithmetic_type_ == Type::NEGATIVE) {
    return;
  }
  Expression *right = expression->right_.get();
  switch (right->type()) {
    case ExprType::ARITHMETIC: {
      find_aggr_func((ArithmeticExpr *)right, exprs);
    }break;
    case ExprType::AGGRFUNCTION: {
      exprs.push_back((AggrFuncExpr *)right);
    }break;
  }
}

// ArithmeticExpr end...
//////////////////////////////////////////////////////////

// AggrFuncExpr start...
AggrFuncExpr::AggrFuncExpr(AggFuncType type, FieldExpr *field) : type_(type), field_(field) {}

AggrFuncExpr::AggrFuncExpr(AggFuncType type, ValueExpr *value) : type_(type),value_(value) {}

AttrType AggrFuncExpr::value_type() const {
  switch(type_) {
    case FUNC_AVG: return AttrType::FLOATS;
    case FUNC_COUNT: return AttrType::INTS;
    case FUNC_MAX: case FUNC_MIN: case FUNC_SUM: {
      if (field_) {
        return field_->field().attr_type();
      }
      return value_->value_type();
    }
  }
  LOG_ERROR("AggFuncExpr's value_type is UNDEFINED, please check expr");
  return AttrType::UNDEFINED;
}

RC AggrFuncExpr::get_value(const Tuple &tuple, Value &value) const {
  if (field_ != nullptr) {
    return tuple.find_cell(TupleCellSpec(field_->table_name(), field_->field_name(), nullptr, type_), value);
  }else if (value_ != nullptr) {
    if (tuple.cell_num() > 0) {
      value = Value("");
      return RC::SUCCESS;
    }
  }
  return RC::UNIMPLENMENT;
}
RC AggrFuncExpr::try_get_value(Value &value) const {
  if (value_ != nullptr) {
    value = Value("");
    return RC::SUCCESS;
  }
  return RC::UNIMPLENMENT;
}

// AggrFuncExpr end...
////////////////////////////////////////////////////

// SubQueryExpr start..
RC SubQueryExpr::get_value(const Tuple &tuple, Value &value) const {
  return RC::UNIMPLENMENT;
}