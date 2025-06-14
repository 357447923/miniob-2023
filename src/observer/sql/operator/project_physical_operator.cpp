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
// Created by WangYunlai on 2022/07/01.
//

#include "common/log/log.h"
#include "sql/operator/project_physical_operator.h"
#include "storage/record/record.h"
#include "storage/table/table.h"

RC ProjectPhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  PhysicalOperator *child = children_[0].get();
  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  return RC::SUCCESS;
}

RC ProjectPhysicalOperator::next()
{
  if (children_.empty()) {
    if (first_enter_) {
      const std::vector<TupleCellSpec *> &speces = tuple_.speces();
      first_enter_ = false;
      if (!speces.empty()) {
        for (auto &spec : speces) {
          Expression *expr = spec->expression();
          if (expr == nullptr || expr->type() != ExprType::FUNC || FuncExpr::check_if_deps_field((FuncExpr *)expr)) {
            return RC::RECORD_EOF;
          }
        }
        return RC::SUCCESS;
      }
    }
    return RC::RECORD_EOF;
  }
  return children_[0]->next();
}

RC ProjectPhysicalOperator::close()
{
  if (!children_.empty()) {
    children_[0]->close();
  }else if (!first_enter_){
    first_leave_ = false;
  }
  return RC::SUCCESS;
}
Tuple *ProjectPhysicalOperator::current_tuple()
{
  if (children_.empty() && !first_enter_ && first_leave_) {
    tuple_.set_tuple(&empty_tuple_);
    return &tuple_;
  }
  tuple_.set_tuple(children_[0]->current_tuple());
  return &tuple_;
}

void ProjectPhysicalOperator::add_projection(const std::vector<Expression *> &exprs) {
  
  for (Expression *expr : exprs) {
    switch (expr->type()) {
      case ExprType::AGGRFUNCTION: {
        AggrFuncExpr *aggr_expr = static_cast<AggrFuncExpr *>(expr);
        TupleCellSpec *spec = nullptr;
        FieldExpr *field_expr = aggr_expr->field();
        if (field_expr != nullptr) {
          Field &field = aggr_expr->field()->field();
          spec = new TupleCellSpec(field.table_name(), field.meta()->name(), field.meta()->name(), aggr_expr->aggr_type());
        }else {
          const Value &value = aggr_expr->value()->get_value();
          spec = new TupleCellSpec(value.data(), aggr_expr->aggr_type());
        }
        tuple_.add_cell_spec(spec, aggr_expr->aggr_type());
      }break;
      case ExprType::FIELD: {
        Field &field = static_cast<FieldExpr *>(expr)->field();
        TupleCellSpec *spec = new TupleCellSpec(field.table_name(), field.field_name(), nullptr);
        tuple_.add_cell_spec(spec, FUNC_NONE);
      }break;
      case ExprType::ARITHMETIC: {
        ArithmeticExpr *arith_expr = static_cast<ArithmeticExpr *>(expr);
        TupleCellSpec *spec = new TupleCellSpec(arith_expr->name().c_str());
        spec->set_expr(arith_expr);
        tuple_.add_cell_spec(spec, FUNC_NONE);
      }break;
      case ExprType::FUNC: {
        FuncExpr *func_expr = static_cast<FuncExpr *>(expr);
        TupleCellSpec *spec = new TupleCellSpec(func_expr->func_type());
        spec->set_expr(func_expr);
        tuple_.add_cell_spec(spec, FUNC_NONE);
      }break;
      default: {
        LOG_WARN("Expression type_id => %d, couldn't parse in ProjectPhysicalOperator.", expr->type());
      }break;
    }
  }
}

void ProjectPhysicalOperator::add_projection(const Field *field)
{
  // 对单表来说，展示的(alias) 字段总是字段名称，
  // 对多表查询来说，展示的alias 需要带表名字
  TupleCellSpec *spec = new TupleCellSpec(field->table_name(), field->meta()->name(), field->meta()->name());
  tuple_.add_cell_spec(spec, field->type());
}

