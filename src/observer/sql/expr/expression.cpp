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

#include <cmath>
#include <iomanip>
#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/select_stmt.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "common/typecast.h"

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

static inline bool have_more_value_after_get(const Tuple &tuple, const SubQueryExpr *sub_query_expr) {
  Value tmp;
  return sub_query_expr->get_value(tuple, tmp) != RC::RECORD_EOF;
}

static RC get_value_from_sub_query(const Tuple &tuple, const SubQueryExpr *sub_query, Value &value) {
  assert(sub_query != nullptr);
  assert(((Expression *)sub_query)->type() == ExprType::SUBQUERY);
  sub_query->open_sub_query();
  RC rc = sub_query->get_value(tuple, value);
  if (rc != RC::SUCCESS) {
    sub_query->close_sub_query();
    return rc;
  }
  if (have_more_value_after_get(tuple, sub_query)) {
    sub_query->close_sub_query();
    LOG_ERROR("OP in SubQuery only have a record, but can get more than one record.");
    return RC::INTERNAL;
  }
  sub_query->close_sub_query();
  return RC::SUCCESS;
}

RC ComparisonExpr::get_value(const Tuple &tuple, Value &value) const
{
  Value left_value;
  Value right_value;

  bool bool_value = false;

  // for [not] exists
  if (comp_ == CompOp::EXISTS_OP || comp_ == CompOp::NOT_EXISTS_OP) {
    assert(right_->type() == ExprType::SUBQUERY);
    const SubQueryExpr *expr = static_cast<const SubQueryExpr *>(right_.get());
    expr->open_sub_query();
    RC rc = expr->get_value(tuple, right_value);
    if (RC::SUCCESS != rc && RC::RECORD_EOF != rc) {
      return rc;
    }
    expr->close_sub_query();
    value.set_boolean(comp_ == CompOp::EXISTS_OP? rc == RC::SUCCESS: rc == RC::RECORD_EOF);
    return RC::SUCCESS;
  }
  // for [not] in
  if (comp_ == CompOp::IN_OP || comp_ == CompOp::NOT_IN_OP) {
    RC rc = left_->get_value(tuple, left_value);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    if (left_value.is_null()) {
      value.set_boolean(false);
      return rc;    // success
    }
    std::vector<Value> right_cells;
    right_cells.emplace_back(Value());
    if (ExprType::SUBQUERY == right_->type()) {
      auto sub_query_expr = (const SubQueryExpr *)right_.get();
      sub_query_expr->open_sub_query();
      while (RC::SUCCESS == (rc = sub_query_expr->get_value(tuple, right_cells.back()))) {
        right_cells.emplace_back(Value());
      }
      sub_query_expr->close_sub_query();
      if (RC::RECORD_EOF != rc) {
        LOG_ERROR("[NOT] IN Get SubQuery Value Failed. RC = %d:%s", rc, strrc(rc));
        return rc;
      }
      right_cells.pop_back();
    }else {
      assert(ExprType::SUBLIST == right_->type());
      auto list_expr = (const ListQueryExpr *)right_.get();
      value.set_boolean(comp_ == CompOp::IN_OP? list_expr->contains(left_value): !list_expr->contains(left_value));
      return RC::SUCCESS;
    }
    
    auto has_null = [](const std::vector<Value> &values) {
      for (auto &value : values) {
        if (value.is_null()) {
          return true;
        }
      }
      return false;
    };
    auto contains = [&left_value](const std::vector<Value> &values) {
      for (auto &value : values) {
        if (value == left_value) {
          return true;
        }
      }
      return false;
    };

    bool res = CompOp::IN_OP == comp_? contains(right_cells)
                                     : (has_null(right_cells)? false: !contains(right_cells));
    value.set_boolean(res);
    return RC::SUCCESS;
  }

  // 普通的比较查询
  // 处理右边是子查询的情况
  if (right_->type() == ExprType::SUBQUERY) {
    auto sub_query = (const SubQueryExpr *)right_.get();
    RC rc = get_value_from_sub_query(tuple, sub_query, right_value);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    if (left_->type() == ExprType::SUBQUERY) {
      sub_query = (const SubQueryExpr *)left_.get();
      rc = get_value_from_sub_query(tuple, sub_query, left_value);
    }else {
      rc = left_->get_value(tuple, left_value);
    }
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }else {
    // 处理右边是非子查询的情况
    RC rc = right_->get_value(tuple, right_value);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    if (left_->type() == ExprType::SUBQUERY) {
      auto subquery = (const SubQueryExpr *)left_.get();
      rc = get_value_from_sub_query(tuple, subquery, left_value);
    }else {
      rc = left_->get_value(tuple, left_value);
    }
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }

  bool res = false;
  RC rc = compare_value(left_value, right_value, res);
  if (rc == RC::SUCCESS) {
    value.set_boolean(res);
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
      TupleCellSpec tmp_spec(type_, value_);
      return tuple.find_cell(tmp_spec, value);
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

SubQueryExpr::SubQueryExpr(SelectStmt *select_stmt) : select_stmt_(select_stmt) {}

SubQueryExpr::~SubQueryExpr() {
  if (select_stmt_ != nullptr) {
      delete select_stmt_;
      select_stmt_ = nullptr;
    }
    if (sub_query_logical_ != nullptr) {
      delete sub_query_logical_;
      sub_query_logical_ = nullptr;
    }
    if (sub_query_operator_ != nullptr) {
      delete sub_query_operator_;
      sub_query_operator_ = nullptr;
    }
}

// SubQueryExpr start...
RC SubQueryExpr::get_value(const Tuple &tuple, Value &value) const {
  assert(nullptr != sub_query_operator_);
  sub_query_operator_->set_parent_tuple(&tuple);
  return get_value(value);
}

RC SubQueryExpr::get_value(Value &value) const {
  RC rc = sub_query_operator_->next();
  if (RC::RECORD_EOF == rc) {
    value.set_null();
  }
  if (RC::SUCCESS != rc) {
    return rc;
  }
  Tuple *child_tuple = sub_query_operator_->current_tuple();
  if (nullptr == child_tuple) {
    LOG_WARN("failed to get current record. rc=%s", strrc(rc));
    return RC::INTERNAL;
  }
  rc = child_tuple->cell_at(0, value);
  return rc;
}

RC SubQueryExpr::open_sub_query() const {
  assert(sub_query_operator_ != nullptr);
  return sub_query_operator_->open(trx_);
}

RC SubQueryExpr::close_sub_query() const {
  assert(sub_query_operator_ != nullptr);
  return sub_query_operator_->close();
}

// SubQueryExpr end...
////////////////////////////////////////////////////

// ListQueryExpr start...
ListQueryExpr::ListQueryExpr(const Value *values, int size) {
  assert(size > 0);
  AttrType type = values[0].attr_type();
  for (int i = 0; i < size; i++) {
    assert(values->attr_type() == type);
    value_list_.insert(values[i]);
  }
}

ListQueryExpr::ListQueryExpr(const std::vector<Value> &values) {
  assert(!values.empty());
  AttrType type = values[0].attr_type();
  for (auto &value : values) {
    assert(value.attr_type() == type);
    value_list_.insert(value);
  }
}

ListQueryExpr::ListQueryExpr(std::unordered_set<Value, Value> &value_list) {
  assert(!value_list_.empty());
  AttrType type = value_list.begin()->attr_type();
  for (auto &value : value_list) {
    assert(value.attr_type() == type);
  }
  value_list_.swap(value_list);
}

FuncExpr::FuncExpr(FuncType func_type, int param_size, Expression *param1, Expression *param2) {
  func_type_ = func_type;
  param_size_ = param_size;
  if (param1 != nullptr) {
    params_expr_.emplace_back(param1);
  }
  if (param2 != nullptr) {
    params_expr_.emplace_back(param2);
  }
}

RC FuncExpr::get_value(const Tuple &tuple, Value &value) const {
  RC rc = RC::SUCCESS;
  switch (func_type_)
  {
  case FUNC_LENGTH: {
    rc = get_func_length_value(tuple, value);
  }break;
  case FUNC_ROUND: {
    rc = get_func_round_value(tuple, value);
  }break;
  case FUNC_DATE_FORMAT: {
    rc = get_func_data_format_value(tuple, value);
  }break;
  default: {
    LOG_ERROR("this FuncExpr is error, because of func_type: %d", func_type_);
    rc = RC::INTERNAL;
  }break;
  }
  return rc;
}

RC FuncExpr::get_func_length_value(const Tuple &tuple, Value &value) const {
  Expression *param_expr = *params_expr_.begin();
  Value param_value;
  param_expr->get_value(tuple, param_value);
  // unsupported not chars
  if (param_value.attr_type() != CHARS) {
    return RC::INTERNAL;
  }
  int result_length = param_value.get_string().length();
  value.set_int(result_length);
  return RC::SUCCESS;
}

RC FuncExpr::get_func_round_value(const Tuple &tuple, Value &value) const {
  if (param_size_ > 1) {
    Expression *param_expr = params_expr_[0];
    Expression *param_expr_precision = params_expr_[1];
    Value param_expr_value;
    Value param_expr_precision_value;
    param_expr->get_value(tuple, param_expr_value);
    param_expr_precision->get_value(tuple, param_expr_precision_value);
    if (param_expr_value.attr_type() != FLOATS 
            || param_expr_precision_value.attr_type() != INTS) {
      return RC::INTERNAL;
    }
    float value_float = param_expr_value.get_float();
    int value_precision = param_expr_precision_value.get_int();
    auto inner_round = [](float f, int precision) {
      std::stringstream ss;
      ss << std::fixed << std::setprecision(precision) << f;
      ss >> f;
      return f;
    };
    value_float = inner_round(value_float, value_precision);
    value.set_float(value_float);
  }else {
    Expression *param_expr = params_expr_[0];
    Value param_expr_value;
    param_expr->get_value(tuple, param_expr_value);
    if (param_expr_value.attr_type() != FLOATS) {
      return RC::INTERNAL;
    }
    float value_float = param_expr_value.get_float();
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << value_float;
    ss >> value_float;
    value.set_float(value_float);
  }
  return RC::SUCCESS;
}

RC FuncExpr::get_func_data_format_value(const Tuple &tuple, Value &value) const {
  const static char *month_str[] = {"", "January", "February", "March", 
      "April", "May", "June", "July", "August", "September",
      "October", "November", "December"};
  Expression *date_expr = params_expr_[0];
  Expression *format_expr = params_expr_[1];
  Value date_expr_value;
  Value format_expr_value;
  date_expr->get_value(tuple, date_expr_value);
  if (date_expr_value.attr_type() == CHARS) {
    if (common::type_cast(date_expr_value, DATES) != RC::SUCCESS) {
      return RC::INTERNAL;
    }
  }
  format_expr->get_value(tuple, format_expr_value);
  if (date_expr_value.attr_type() != DATES
      || format_expr_value.attr_type() != CHARS) {
    return RC::INTERNAL;
  }
  int32_t value_date = date_expr_value.get_date();
  string value_format_str = format_expr_value.get_string();
  string result_date_str;
  int year = value_date / 10000;
  int month = (value_date / 100) % 100;
  int day = value_date % 100;
  for (size_t i = 0; i < value_format_str.length(); ++i) {
    if ('A' <= value_format_str[i] && value_format_str[i] <= 'z') {
      switch (value_format_str[i]) {
        case 'Y': {
          char tmp[5];
          sprintf(tmp, "%d", year);
          result_date_str += tmp;
          break;
        }
        case 'y': {
          char tmp[5];
          sprintf(tmp, "%d", year % 100);
          if (0 <= (year % 100) && (year % 100) <= 9) {
            result_date_str += "0";
          }
          result_date_str += tmp;
          break;
        }
        case 'M': {
          if (month <= 0 || month >= 13) {
            return RC::INTERNAL;
          }
          result_date_str += month_str[month];
          break;
        }
        case 'm': {
          char tmp[3];
          sprintf(tmp, "%d", month);
          if (0 <= month && month <= 9) {
            result_date_str += "0";
          }
          result_date_str += tmp;
          break;
        }
        case 'D': {
          char tmp[3];
          sprintf(tmp, "%d", day);
          if (10 <= day && day <= 20) {
            result_date_str += tmp;
            result_date_str += "th";
          }else {
            switch (day % 10) {
              case 1: {
                result_date_str += tmp;
                result_date_str += "st";
                break;
              }
              case 2: {
                result_date_str += tmp;
                result_date_str += "nd";
                break;
              }
              case 3: {
                result_date_str += tmp;
                result_date_str += "rd";
                break;
              }
              default: {
                result_date_str += tmp;
                result_date_str += "th";
                break;
              }
            }
          }
          break;
        }
        case 'd': {
          char tmp[3];
          sprintf(tmp, "%d", day);
          if (0 <= day && day <= 9) {
            result_date_str += "0";
          }
          result_date_str += tmp;
          break;
        }
        default: {
          result_date_str += value_format_str[i];
          break;
        }
      }
    }else if (value_format_str[i] != '%') {
      result_date_str += value_format_str[i];
    }
  }
  value.set_string(strdup(result_date_str.c_str()), result_date_str.length());
  return RC::SUCCESS;
}

RC FuncExpr::find_field_need(const Table *table, FuncExpr *func_expr) {
  const std::vector<Expression *> &params = func_expr->get_params();
  for (auto param : params) {
    if (param->type() == ExprType::FIELD) {
      FieldExpr *field_param = (FieldExpr *)param;
      const FieldMeta *field_meta = table->table_meta().field(field_param->name().c_str());
      if (!field_meta) {
        return RC::SCHEMA_FIELD_NOT_EXIST;
      }
      Field &field = field_param->field();
      field.set_field(field_meta);
      field.set_table(table);
    }else if (param->type() == ExprType::SUBQUERY) {
      LOG_ERROR("unimplement function when func's param type is subquery");
      return RC::UNIMPLENMENT;
    }
  }
  return RC::SUCCESS;
}

RC FuncExpr::find_field_need(const std::unordered_map<std::string, Table *> &table_map, FuncExpr *func_expr) {
  const std::vector<Expression *> &params = func_expr->get_params();
  for (auto param : params) {
    if (param->type() == ExprType::FIELD) {
      if (!create_field_by_expr((FieldExpr *)param, table_map)) {
        LOG_ERROR("create field expr error in FuncExpr::find_field_need");
        return RC::INTERNAL;
      }
    }else if (param->type() == ExprType::SUBQUERY) {
      LOG_ERROR("unimplement function when func's param type is subquery");
      return RC::UNIMPLENMENT;
    }
  }
  return RC::SUCCESS;
}

bool FuncExpr::check_if_deps_field(FuncExpr *func_expr) {
  assert(func_expr != nullptr);
  const std::vector<Expression *> &exprs = func_expr->get_params();
  Value tmp;
  for (auto expr : exprs) {
    if (RC::SUCCESS != expr->try_get_value(tmp)) {
      return true;
    }
  }
  return false;
}
