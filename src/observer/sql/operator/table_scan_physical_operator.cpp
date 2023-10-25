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
// Created by WangYunlai on 2021/6/9.
//

#include "sql/operator/table_scan_physical_operator.h"
#include "storage/table/table.h"
#include "event/sql_debug.h"

using namespace std;

static bool ensure_subquery_enable(SubQueryExpr *subquery_expr, const CompOp comp_op) {
  if (comp_op != CompOp::EXISTS_OP && comp_op != CompOp::NOT_EXISTS_OP 
          && comp_op != CompOp::IN_OP && comp_op != CompOp::NOT_IN_OP) {
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

RC TableScanPhysicalOperator::open(Trx *trx)
{
  RC rc = table_->get_record_scanner(record_scanner_, trx, readonly_);
  if (rc == RC::SUCCESS) {
    tuple_.set_schema(table_, table_->table_meta().field_metas());
  }
  trx_ = trx;
  for (auto &expr : predicates_) {
    if (expr->type() == ExprType::COMPARISON) {
      ComparisonExpr *expression = (ComparisonExpr *)expr.get();
      std::unique_ptr<Expression> &left_sub_query = expression->left();
      if (left_sub_query->type() == ExprType::SUBQUERY) {
        ((SubQueryExpr *)left_sub_query.get())->set_trx(trx);
        if (!ensure_subquery_enable((SubQueryExpr *)left_sub_query.get(), expression->comp())) {
          return RC::INTERNAL;
        }
        continue;
      }
      std::unique_ptr<Expression> &right_sub_query = expression->right();
      if (right_sub_query->type() == ExprType::SUBQUERY) {
        ((SubQueryExpr *)right_sub_query.get())->set_trx(trx);
        if (!ensure_subquery_enable((SubQueryExpr *)right_sub_query.get(), expression->comp())) {
          return RC::INTERNAL;
        }
        continue;
      }
    }
  }
  return rc;
}

RC TableScanPhysicalOperator::next()
{
  if (!record_scanner_.has_next()) {
    return RC::RECORD_EOF;
  }

  RC rc = RC::SUCCESS;
  bool filter_result = false;
  while (record_scanner_.has_next()) {
    rc = record_scanner_.next(current_record_);
    // 设置位图长度
    LOG_INFO("load table: %s",table_->table_meta().name());
    int bitmap_len = common::bitmap_size(table_->table_meta().field_metas()->size());
    current_record_.set_bitmap_len(bitmap_len);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    tuple_.set_record(&current_record_);
    rc = filter(tuple_, filter_result);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    if (filter_result) {
      sql_debug("get a tuple: %s", tuple_.to_string().c_str());
      break;
    } else {
      sql_debug("a tuple is filtered: %s", tuple_.to_string().c_str());
      rc = RC::RECORD_EOF;
    }
  }
  return rc;
}

RC TableScanPhysicalOperator::close()
{
  return record_scanner_.close_scan();
}

Tuple *TableScanPhysicalOperator::current_tuple()
{
  tuple_.set_record(&current_record_);
  return &tuple_;
}

string TableScanPhysicalOperator::param() const
{
  return table_->name();
}

void TableScanPhysicalOperator::set_predicates(vector<unique_ptr<Expression>> &&exprs)
{
  predicates_ = std::move(exprs);
}

RC TableScanPhysicalOperator::filter(RowTuple &tuple, bool &result)
{
  RC rc = RC::SUCCESS;
  Value value;
  for (unique_ptr<Expression> &expr : predicates_) {
    rc = expr->get_value(tuple, value);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    bool tmp_result = value.get_boolean();
    if (!tmp_result) {
      result = false;
      return rc;
    }
  }

  result = true;
  return rc;
}
