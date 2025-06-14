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
// Created by Wangyunlai on 2022/12/14.
//

#include <utility>

#include "common/log/log.h"
#include "sql/expr/expression.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/calc_physical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/delete_physical_operator.h"
#include "sql/operator/explain_logical_operator.h"
#include "sql/operator/explain_physical_operator.h"
#include "sql/operator/groupby_logical_operator.h"
#include "sql/operator/groupby_physical_operator.h"
#include "sql/operator/index_scan_physical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/insert_physical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/join_physical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/predicate_physical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/project_physical_operator.h"
#include "sql/operator/sorted_logical_operator.h"
#include "sql/operator/sorted_physical_operator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/table_scan_physical_operator.h"
#include "sql/operator/update_logical_operator.h"
#include "sql/operator/update_physical_operator.h"
#include "sql/operator/create_table_select_logical_operator.h"
#include "sql/operator/create_table_select_physical_operator.h"
#include "sql/optimizer/physical_plan_generator.h"

using namespace std;

RC PhysicalPlanGenerator::create(LogicalOperator& logical_operator, unique_ptr<PhysicalOperator>& oper) {
    RC rc = RC::SUCCESS;

    switch (logical_operator.type()) {
        case LogicalOperatorType::CALC: {
            return create_plan(static_cast<CalcLogicalOperator&>(logical_operator), oper);
        } break;

        case LogicalOperatorType::TABLE_GET: {
            return create_plan(static_cast<TableGetLogicalOperator&>(logical_operator), oper);
        } break;

        case LogicalOperatorType::PREDICATE: {
            return create_plan(static_cast<PredicateLogicalOperator&>(logical_operator), oper);
        } break;

        case LogicalOperatorType::PROJECTION: {
            return create_plan(static_cast<ProjectLogicalOperator&>(logical_operator), oper);
        } break;

        case LogicalOperatorType::GROUP: {
            return create_plan(static_cast<GroupByLogicalOperator&>(logical_operator), oper);
        } break;

        case LogicalOperatorType::INSERT: {
            return create_plan(static_cast<InsertLogicalOperator&>(logical_operator), oper);
        } break;

        case LogicalOperatorType::UPDATE: {
            return create_plan(static_cast<UpdateLogicalOperator&>(logical_operator), oper);
        } break;

        case LogicalOperatorType::DELETE: {
            return create_plan(static_cast<DeleteLogicalOperator&>(logical_operator), oper);
        } break;

        case LogicalOperatorType::EXPLAIN: {
            return create_plan(static_cast<ExplainLogicalOperator&>(logical_operator), oper);
        } break;

        case LogicalOperatorType::JOIN: {
            return create_plan(static_cast<JoinLogicalOperator&>(logical_operator), oper);
        } break;

        case LogicalOperatorType::SORTED: {
            return create_plan(static_cast<SortedLogicalOperator&>(logical_operator), oper);
        } break;
        
        case LogicalOperatorType::CREATE_TABLE_SELECT: {
            return create_plan(static_cast<CreateTableSelectLogicalOperator&>(logical_operator), oper);
        } break;

        default: {
            return RC::INVALID_ARGUMENT;
        }
    }
  return rc;
}
// 仅仅支持where子句中的子查询
RC PhysicalPlanGenerator::create_subquery(Expression *expr) {
  assert(expr != nullptr);
  if (expr->type() != ExprType::SUBQUERY) {
    return RC::SUCCESS;
  }
  SubQueryExpr *subquery_expr = (SubQueryExpr *)expr;
  if (subquery_expr->sub_query_operator()) {
    return RC::SUCCESS;
  }
  FilterStmt *sub_filter_stmt = subquery_expr->select_stmt()->filter_stmt();
  if (sub_filter_stmt) {
    const std::vector<FilterUnit *> &filter_units = sub_filter_stmt->filter_units();
    RC rc = RC::SUCCESS;
    for (auto &unit : filter_units) {
      const FilterObj &left = unit->left();
      if (left.is_attr) {
        rc = create_subquery(left.expression);
        if (rc != RC::SUCCESS) {
          return rc;
        }
      }
      const FilterObj &right = unit->right();
      if (right.is_attr) {
        rc = create_subquery(right.expression);
        if (rc != RC::SUCCESS) {
          return rc;
        }
      }
    }
  }
  unique_ptr<PhysicalOperator> subquery_phy_oper;
  RC rc = create_plan(*subquery_expr->sub_query_logical(), subquery_phy_oper);
  if (rc != RC::SUCCESS) {
      LOG_ERROR("subquery physical operator create error, please check by debug");
      return rc;
  }
  subquery_expr->set_sub_query_operator(static_cast<ProjectPhysicalOperator *>(subquery_phy_oper.get()));
  subquery_phy_oper.release();
  return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_subquery(unique_ptr<Expression> &expr) {
  // 处理过滤条件中的子查询, 用子查询的逻辑算子创建出物理算子
  RC rc = RC::SUCCESS;
  if (expr->type() == ExprType::SUBQUERY) {
    SubQueryExpr *subquery_expr = static_cast<SubQueryExpr *>(expr.get());
    create_subquery(subquery_expr);
  }
  return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_plan(TableGetLogicalOperator &table_get_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<Expression>> &predicates = table_get_oper.predicates();
  // 看看是否有可以用于索引查找的表达式
  Table *table = table_get_oper.table();

  Index *index = nullptr;
  ValueExpr *value_expr = nullptr;
  for (auto &expr : predicates) {
    if (expr->type() == ExprType::COMPARISON) {
      auto comparison_expr = static_cast<ComparisonExpr *>(expr.get());
      // 简单处理，就找等值查询
      if (comparison_expr->comp() != EQUAL_TO) {
        continue;
      }

      unique_ptr<Expression> &left_expr = comparison_expr->left();
      unique_ptr<Expression> &right_expr = comparison_expr->right();
      // 左右比较的一边最少是一个值
      if (left_expr->type() != ExprType::VALUE && right_expr->type() != ExprType::VALUE) {
        continue;
      }

      FieldExpr *field_expr = nullptr;
      if (left_expr->type() == ExprType::FIELD) {
        ASSERT(right_expr->type() == ExprType::VALUE, "right expr should be a value expr while left is field expr");
        field_expr = static_cast<FieldExpr *>(left_expr.get());
        value_expr = static_cast<ValueExpr *>(right_expr.get());
      } else if (right_expr->type() == ExprType::FIELD) {
        ASSERT(left_expr->type() == ExprType::VALUE, "left expr should be a value expr while right is a field expr");
        field_expr = static_cast<FieldExpr *>(right_expr.get());
        value_expr = static_cast<ValueExpr *>(left_expr.get());
      }

      if (field_expr == nullptr) {
        continue;
      }

      const Field &field = field_expr->field();
      index = table->find_index_by_field(field.field_name());
      if (nullptr != index) {
        break;
      }
    }
  }

  if (index != nullptr) {
    ASSERT(value_expr != nullptr, "got an index but value expr is null ?");

    const Value &value = value_expr->get_value();
    IndexScanPhysicalOperator *index_scan_oper = new IndexScanPhysicalOperator(
          table, index, table_get_oper.readonly(), 
          &value, true /*left_inclusive*/, 
          &value, true /*right_inclusive*/);
          
    index_scan_oper->set_predicates(std::move(predicates));
    oper = unique_ptr<PhysicalOperator>(index_scan_oper);
    LOG_TRACE("use index scan");
  } else {
    auto table_scan_oper = new TableScanPhysicalOperator(table, table_get_oper.readonly());
    for (auto &predicate : predicates) {
      RC rc;
      switch (predicate->type()) {
        case ExprType::COMPARISON: {
          ComparisonExpr *comparison_expr = (ComparisonExpr *)predicate.get();
          std::unique_ptr<Expression> &sub_query = comparison_expr->left();
          if (sub_query->type() == ExprType::SUBQUERY) {
            rc = create_subquery(sub_query);
          }else {
            std::unique_ptr<Expression> &sub_query = comparison_expr->right();
            rc = create_subquery(sub_query);
          }
        }break;
        case ExprType::CONJUNCTION: {
          for (auto &expr : ((ConjunctionExpr*)predicate.get())->children()) {
            rc = create_subquery(expr);
            if (rc != RC::SUCCESS) {
              break;
            }
          }
        }break;
        default: {
          rc = RC::INTERNAL;
          LOG_ERROR("unknow how to create sub query");
        }break;
      }
      if (rc != RC::SUCCESS) {
        return rc;
      }
    }
    table_scan_oper->set_predicates(std::move(predicates));
    oper = unique_ptr<PhysicalOperator>(table_scan_oper);
    LOG_TRACE("use table scan");
  }

  return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_child_subquery_for_comparison(ComparisonExpr *expr) {
  RC rc = RC::SUCCESS;
  std::unique_ptr<Expression> &left_sub_query = expr->left();
  if (left_sub_query->type() == ExprType::SUBQUERY) {
    rc = create_subquery(left_sub_query);
  }
  if (rc != RC::SUCCESS) {
    LOG_ERROR("create sub query physical operator error");
    return rc;
  }
  std::unique_ptr<Expression> &right_sub_query = expr->right();
  if (right_sub_query->type() == ExprType::SUBQUERY) {
    rc = create_subquery(right_sub_query);
  }
  if (rc != RC::SUCCESS) {
    LOG_ERROR("create sub query physical operator error");
    return rc;
  }
  return rc;
}

RC PhysicalPlanGenerator::create_plan(PredicateLogicalOperator &pred_oper, unique_ptr<PhysicalOperator> &oper)
{
  vector<unique_ptr<LogicalOperator>> &children_opers = pred_oper.children();
  ASSERT(children_opers.size() == 1, "predicate logical operator's sub oper number should be 1");

  LogicalOperator &child_oper = *children_opers.front();

  unique_ptr<PhysicalOperator> child_phy_oper;
  RC rc = create(child_oper, child_phy_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create child operator of predicate operator. rc=%s", strrc(rc));
    return rc;
  }

  vector<unique_ptr<Expression>> &expressions = pred_oper.expressions();
  ASSERT(expressions.size() == 1, "predicate logical operator's children should be 1");

  unique_ptr<Expression> expression = std::move(expressions.front());
  if (expression->type() == ExprType::COMPARISON) {
    ComparisonExpr *expr = (ComparisonExpr *)expression.get();
    rc = create_child_subquery_for_comparison(expr);
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }else if (expression->type() == ExprType::CONJUNCTION) {
    // 似乎对于联结表达式来说，子表达式永远都是比较表达式
    // 下面的断言用来证明这个猜想
    ConjunctionExpr *expr = (ConjunctionExpr *)expression.get();
    for (auto &expression : expr->children()) {
      assert(expression->type() == ExprType::COMPARISON);
      rc = create_child_subquery_for_comparison((ComparisonExpr *)expression.get());
      if (rc != RC::SUCCESS) {
        LOG_ERROR("create sub query physical operator error");
        return rc;
      }
    }
  }
  oper = unique_ptr<PhysicalOperator>(new PredicatePhysicalOperator(std::move(expression)));
  oper->add_child(std::move(child_phy_oper));
  return rc;
}

RC PhysicalPlanGenerator::create_plan(ProjectLogicalOperator& project_oper, unique_ptr<PhysicalOperator>& oper) {
    vector<unique_ptr<LogicalOperator>>& child_opers = project_oper.children();

    unique_ptr<PhysicalOperator> child_phy_oper;

    RC rc = RC::SUCCESS;
    if (!child_opers.empty()) {
        LogicalOperator* child_oper = child_opers.front().get();
        rc = create(*child_oper, child_phy_oper);
        if (rc != RC::SUCCESS) {
            LOG_WARN("failed to create project logical operator's child physical operator. rc=%s", strrc(rc));
            return rc;
        }
    }
    const std::vector<Expression*>* exprs = project_oper.schemas();
    ProjectPhysicalOperator* project_operator = new ProjectPhysicalOperator;

    project_operator->add_projection(*exprs);

    if (child_phy_oper) {
        project_operator->add_child(std::move(child_phy_oper));
    }

    oper = unique_ptr<PhysicalOperator>(project_operator);

    LOG_TRACE("create a project physical operator");
    return rc;
}

RC PhysicalPlanGenerator::create_plan(GroupByLogicalOperator& groupby_oper, std::unique_ptr<PhysicalOperator>& oper) {
    vector<unique_ptr<LogicalOperator>>& child_opers = groupby_oper.children();

    unique_ptr<PhysicalOperator> child_phy_oper;
    RC rc = RC::SUCCESS;
    if (!child_opers.empty()) {
        LogicalOperator* child_oper = child_opers.front().get();
        rc = create(*child_oper, child_phy_oper);
        if (rc != RC::SUCCESS) {
            LOG_WARN("failed to create groupby logical operator's child physical operator. rc=%s", strrc(rc));
            return rc;
        }
    }

    GroupByPhysicalOperator* groupby_operator = new GroupByPhysicalOperator(&groupby_oper.groupby_stmt()->groupby_units(),
                                                                            groupby_oper.aggr_exprs(), groupby_oper.field_exprs());
    if (child_phy_oper) {
        groupby_operator->add_child(std::move(child_phy_oper));
    }
    oper = unique_ptr<PhysicalOperator>(groupby_operator);

    return rc;
}

RC PhysicalPlanGenerator::create_plan(SortedLogicalOperator& sorted_oper,
                                      std::unique_ptr<PhysicalOperator>& oper) {
    if (sorted_oper.units() == nullptr) {
        LOG_ERROR("Can't find OrderUnits from sorted_oper. Create SortedPhysicalOperator failed!");
        return RC::INTERNAL;
    }
    vector<unique_ptr<LogicalOperator>>& childs = sorted_oper.children();
    unique_ptr<PhysicalOperator> child_phy_oper;
    RC rc = RC::SUCCESS;
    if (!childs.empty()) {
        LogicalOperator* child_oper = childs[0].get();
        rc = create(*child_oper, child_phy_oper);
        if (rc != RC::SUCCESS) {
            LOG_WARN("failed to create sorted logical's child. rc=%s", strrc(rc));
        }
    }

    SortedPhysicalOperator* sorted_operator = new SortedPhysicalOperator(sorted_oper.units());
    if (child_phy_oper) {
        sorted_operator->add_child(std::move(child_phy_oper));
    }
    oper.reset(sorted_operator);
    return rc;
}

RC PhysicalPlanGenerator::create_plan(InsertLogicalOperator& insert_oper, unique_ptr<PhysicalOperator>& oper) {
    Table* table = insert_oper.table();
    const std::vector<std::vector<Value>>* values = insert_oper.values();
    InsertPhysicalOperator* insert_phy_oper = new InsertPhysicalOperator(table, values);
    oper.reset(insert_phy_oper);
    return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_plan(UpdateLogicalOperator& update_oper, std::unique_ptr<PhysicalOperator>& oper) {
    vector<unique_ptr<LogicalOperator>>& child_opers = update_oper.children();

    unique_ptr<PhysicalOperator> child_physical_oper;
    RC rc = RC::SUCCESS;
    if (!child_opers.empty()) {
        LogicalOperator* child_oper = child_opers.front().get();
        rc = create(*child_oper, child_physical_oper);
        if (rc != RC::SUCCESS) {
            LOG_WARN("failed to create physical operator. rc=%s", strrc(rc));
            return rc;
        }
    }
    // 构建子查询的物理算子
    for (auto &pair : update_oper.update_map()) {
      Expression *expr = pair.second;
      if (expr->type() == ExprType::SUBQUERY) {
        SubQueryExpr *sub_query = static_cast<SubQueryExpr *>(expr);
        unique_ptr<PhysicalOperator> sub_query_physical_operator;
        rc = create_plan(*sub_query->sub_query_logical(), sub_query_physical_operator);
        if (rc != RC::SUCCESS) {
          LOG_INFO("create subquery fail. file: %s, line: %d", __FILE__, __LINE__);
          return rc;
        }
        // 让表达式管理逻辑算子
        sub_query->set_sub_query_operator(static_cast<ProjectPhysicalOperator *>(sub_query_physical_operator.get()));
        sub_query_physical_operator.release();
      }
    }
    oper = unique_ptr<PhysicalOperator>(new UpdatePhysicalOperator(update_oper.table(), update_oper.update_map()));
    if (child_physical_oper) {
        oper->add_child(std::move(child_physical_oper));
    }
    return RC::SUCCESS;
}

RC PhysicalPlanGenerator::create_plan(DeleteLogicalOperator& delete_oper, unique_ptr<PhysicalOperator>& oper) {
    vector<unique_ptr<LogicalOperator>>& child_opers = delete_oper.children();

    unique_ptr<PhysicalOperator> child_physical_oper;

    RC rc = RC::SUCCESS;
    if (!child_opers.empty()) {
        LogicalOperator* child_oper = child_opers.front().get();
        rc = create(*child_oper, child_physical_oper);
        if (rc != RC::SUCCESS) {
            LOG_WARN("failed to create physical operator. rc=%s", strrc(rc));
            return rc;
        }
    }

    oper = unique_ptr<PhysicalOperator>(new DeletePhysicalOperator(delete_oper.table()));

    if (child_physical_oper) {
        oper->add_child(std::move(child_physical_oper));
    }
    return rc;
}

RC PhysicalPlanGenerator::create_plan(ExplainLogicalOperator& explain_oper, unique_ptr<PhysicalOperator>& oper) {
    vector<unique_ptr<LogicalOperator>>& child_opers = explain_oper.children();

    RC rc = RC::SUCCESS;
    unique_ptr<PhysicalOperator> explain_physical_oper(new ExplainPhysicalOperator);
    for (unique_ptr<LogicalOperator>& child_oper : child_opers) {
        unique_ptr<PhysicalOperator> child_physical_oper;
        rc = create(*child_oper, child_physical_oper);
        if (rc != RC::SUCCESS) {
            LOG_WARN("failed to create child physical operator. rc=%s", strrc(rc));
            return rc;
        }

        explain_physical_oper->add_child(std::move(child_physical_oper));
    }

    oper = std::move(explain_physical_oper);
    return rc;
}

RC PhysicalPlanGenerator::create_plan(JoinLogicalOperator& join_oper, unique_ptr<PhysicalOperator>& oper) {
    RC rc = RC::SUCCESS;
    vector<unique_ptr<Expression>>& predicates = join_oper.predicates();
    vector<unique_ptr<LogicalOperator>>& child_opers = join_oper.children();
    if (child_opers.size() != 2) {
        LOG_WARN("join operator should have 2 children, but have %d", child_opers.size());
        return RC::INTERNAL;
    }
    NestedLoopJoinPhysicalOperator* nestedLoopJoinPhysicalOperator = new NestedLoopJoinPhysicalOperator;
    nestedLoopJoinPhysicalOperator->set_predicates(std::move(predicates));
    unique_ptr<PhysicalOperator> join_physical_oper(nestedLoopJoinPhysicalOperator);
    for (auto& child_oper : child_opers) {
        unique_ptr<PhysicalOperator> child_physical_oper;
        rc = create(*child_oper, child_physical_oper);
        if (rc != RC::SUCCESS) {
            LOG_WARN("failed to create physical child oper. rc=%s", strrc(rc));
            return rc;
        }
        join_physical_oper->add_child(std::move(child_physical_oper));
    }

    oper = std::move(join_physical_oper);
    return rc;
}

RC PhysicalPlanGenerator::create_plan(CalcLogicalOperator& logical_oper, std::unique_ptr<PhysicalOperator>& oper) {
    RC rc = RC::SUCCESS;
    CalcPhysicalOperator* calc_oper = new CalcPhysicalOperator(std::move(logical_oper.expressions()));
    oper.reset(calc_oper);
    return rc;
}

RC PhysicalPlanGenerator::create_plan(CreateTableSelectLogicalOperator &logical_oper, 
    std::unique_ptr<PhysicalOperator> &oper) {
  RC rc = RC::SUCCESS;
  std::vector<unique_ptr<LogicalOperator>> &children = logical_oper.children();
  assert(children.size() == 1);
  unique_ptr<PhysicalOperator> physical_oper = unique_ptr<PhysicalOperator>(new CreateTableSelectPhysicalOperator);
  for (auto &child_oper : children) {
    unique_ptr<PhysicalOperator> child_physical_oper;
    rc = create(*child_oper, child_physical_oper);
    if (rc != RC::SUCCESS) {
        LOG_WARN("failed to create physical child oper. rc=%s", strrc(rc));
        return rc;
    }
    physical_oper->add_child(std::move(child_physical_oper));
  }
  
  CreateTableSelectPhysicalOperator *create_oper = (CreateTableSelectPhysicalOperator *)(physical_oper.get());
  create_oper->set_create_table_stmt(logical_oper.create_table_stmt());
  create_oper->set_db(logical_oper.db());
  oper = std::move(physical_oper);
  return rc;
}