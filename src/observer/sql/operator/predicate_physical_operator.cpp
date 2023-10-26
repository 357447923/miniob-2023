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
// Created by WangYunlai on 2022/6/27.
//

#include "common/log/log.h"
#include "sql/operator/predicate_physical_operator.h"
#include "storage/record/record.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/field/field.h"
#include "sql/expr/expression.h"

PredicatePhysicalOperator::PredicatePhysicalOperator(std::unique_ptr<Expression> expr) : expression_(std::move(expr))
{
  ASSERT(expression_->value_type() == BOOLEANS, "predicate's expression should be BOOLEAN type");
}

static bool ensure_subquery_enable(SubQueryExpr *subquery_expr, const CompOp comp_op) {
  if (comp_op == CompOp::EQUAL_TO) {
    subquery_expr->open_sub_query();
    Value tmp;
    RC rc = subquery_expr->get_value(tmp);
    if (rc != RC::SUCCESS) {
      subquery_expr->close_sub_query();
      return false;
    }
    rc = subquery_expr->get_value(tmp);
    if (rc != RC::RECORD_EOF) {
      subquery_expr->close_sub_query();
      return false;
    }
    subquery_expr->close_sub_query();
  }
  return true;
}

RC PredicatePhysicalOperator::open(Trx *trx)
{
  if (children_.size() != 1) {
    LOG_WARN("predicate operator must has one child");
    return RC::INTERNAL;
  }
  if (expression_->type() == ExprType::CONJUNCTION) {
    for (auto &expr : ((ConjunctionExpr *)expression_.get())->children()) {
      ComparisonExpr *comparison_expr = ((ComparisonExpr *)expr.get());
      std::unique_ptr<Expression> &left_sub_expr = comparison_expr->left();
      if (left_sub_expr->type() == ExprType::SUBQUERY) {
        ((SubQueryExpr *)left_sub_expr.get())->set_trx(trx);
        if (!ensure_subquery_enable((SubQueryExpr *)left_sub_expr.get(), comparison_expr->comp())) {
          return RC::INTERNAL;
        }
        continue;
      }
      std::unique_ptr<Expression> &right_sub_expr = comparison_expr->right();
      if (right_sub_expr->type() == ExprType::SUBQUERY) {
        ((SubQueryExpr *)right_sub_expr.get())->set_trx(trx);
        if (!ensure_subquery_enable((SubQueryExpr *)right_sub_expr.get(), comparison_expr->comp())) {
          return RC::INTERNAL;
        }
        continue;
      }
    }
  }else if (expression_->type() == ExprType::COMPARISON) {
    ComparisonExpr *comparison_expr = ((ComparisonExpr *)expression_.get());
    std::unique_ptr<Expression> &left_sub_expr = comparison_expr->left();
    if (left_sub_expr->type() == ExprType::SUBQUERY) {
      ((SubQueryExpr *)left_sub_expr.get())->set_trx(trx);
      if (!ensure_subquery_enable((SubQueryExpr *)left_sub_expr.get(), comparison_expr->comp())) {
        return RC::INTERNAL;
      }
    }
    std::unique_ptr<Expression> &right_sub_expr = comparison_expr->right();
    if (right_sub_expr->type() == ExprType::SUBQUERY) {
      ((SubQueryExpr *)right_sub_expr.get())->set_trx(trx);
      if (!ensure_subquery_enable((SubQueryExpr *)right_sub_expr.get(), comparison_expr->comp())) {
        return RC::INTERNAL;
      }
    }
  }
  return children_[0]->open(trx);
}

RC PredicatePhysicalOperator::next()
{
  RC rc = RC::SUCCESS;
  PhysicalOperator *oper = children_.front().get();

  while (RC::SUCCESS == (rc = oper->next())) {
    Tuple *tuple = oper->current_tuple();
    if (nullptr == tuple) {
      rc = RC::INTERNAL;
      LOG_WARN("failed to get tuple from operator");
      break;
    }

    Value value;
    JoinedTuple joined_tuple;
    if (parent_tuple_ != nullptr) {
      joined_tuple.set_left(tuple);
      // 这里只用于查询, 没有用于其他操作
      joined_tuple.set_right(const_cast<Tuple *>(parent_tuple_));
      tuple = &joined_tuple;
    }
    rc = expression_->get_value(*tuple, value);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    if (value.get_boolean()) {
      return rc;
    }
  }
  return rc;
}

RC PredicatePhysicalOperator::close()
{
  children_[0]->close();
  return RC::SUCCESS;
}

Tuple *PredicatePhysicalOperator::current_tuple()
{
  return children_[0]->current_tuple();
}
