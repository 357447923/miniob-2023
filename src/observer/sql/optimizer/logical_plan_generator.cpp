/* Copyright (c) 2023 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/08/16.
//

#include "sql/optimizer/logical_plan_generator.h"

#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/explain_logical_operator.h"
#include "sql/operator/groupby_logical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/sorted_logical_operator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/update_logical_operator.h"

#include "sql/stmt/calc_stmt.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/explain_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/group_by_stmt.h"
#include "sql/stmt/insert_stmt.h"
#include "sql/stmt/orderby_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/stmt.h"
#include "sql/stmt/update_stmt.h"

using namespace std;

RC LogicalPlanGenerator::create(Stmt* stmt, unique_ptr<LogicalOperator>& logical_operator) {
    RC rc = RC::SUCCESS;
    switch (stmt->type()) {
        case StmtType::CALC: {
            CalcStmt* calc_stmt = static_cast<CalcStmt*>(stmt);
            rc = create_plan(calc_stmt, logical_operator);
        } break;

        case StmtType::SELECT: {
            SelectStmt* select_stmt = static_cast<SelectStmt*>(stmt);
            rc = create_plan(select_stmt, logical_operator);
        } break;

        case StmtType::INSERT: {
            InsertStmt* insert_stmt = static_cast<InsertStmt*>(stmt);
            rc = create_plan(insert_stmt, logical_operator);
        } break;

        case StmtType::DELETE: {
            DeleteStmt* delete_stmt = static_cast<DeleteStmt*>(stmt);
            rc = create_plan(delete_stmt, logical_operator);
        } break;

        case StmtType::UPDATE: {
            UpdateStmt* update_stmt = static_cast<UpdateStmt*>(stmt);
            std::unordered_map <std::string, Value*> map = update_stmt->update_map();
            Value *test_value_ptr = map["id2"];
            test_value_ptr = map["id1"];
            rc = create_plan(update_stmt, logical_operator);
        } break;

        case StmtType::EXPLAIN: {
            ExplainStmt* explain_stmt = static_cast<ExplainStmt*>(stmt);
            rc = create_plan(explain_stmt, logical_operator);
        } break;
        default: {
            rc = RC::UNIMPLENMENT;
        }
    }
    return rc;
}

RC LogicalPlanGenerator::create_plan(CalcStmt* calc_stmt, std::unique_ptr<LogicalOperator>& logical_operator) {
    logical_operator.reset(new CalcLogicalOperator(std::move(calc_stmt->expressions())));
    return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    SelectStmt* select_stmt,
    unique_ptr<LogicalOperator>& logical_operator) {
    unique_ptr<LogicalOperator> table_oper(nullptr);

    const std::vector<Table*>& tables = select_stmt->tables();
    const std::vector<Expression*>& expressions = select_stmt->query_expressions();
    if (tables.empty()) {
        // check all expression are FuncExpr when tables is empty
        for (auto &expr : expressions) {
            if (expr->type() != ExprType::FUNC) {
                LOG_ERROR("expression is not funcExpr when tables is empty");
                return RC::INTERNAL;
            }
        }
        unique_ptr<LogicalOperator> project_oper(new ProjectLogicalOperator(&expressions));
        logical_operator = std::move(project_oper);
        return RC::SUCCESS;
    }
    int index = -1;
    for (Table* table : tables) {
        std::vector<Field> fields;
        for (const Expression* expression : expressions) {
            if (expression->type() == ExprType::FIELD) {
                Field& field = ((FieldExpr*)(expression))->field();
                if (0 == strcmp(field.table_name(), table->name())) {
                    fields.push_back(field);
                }
            }
        }

        unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, fields, true /*readonly*/));
        if (table_oper == nullptr) {
            table_oper = std::move(table_get_oper);
        } else {
            JoinLogicalOperator* join_oper = new JoinLogicalOperator;
            join_oper->add_child(std::move(table_oper));
            join_oper->add_child(std::move(table_get_oper));
            // on -> 谓词操作符
            //     new JoinLogicalOperator                 <- table_oper (3)
            //          /              \
            //         /          on_predicate_oper        <- table_oper (2)
            //        /                  |
            // table_get_oper  new JoinLogicalOperator
            //                      /             \
            // table_oper (1) ->  table_oper       table_get_oper
            if (select_stmt->inner_join_filter_stmts().size() != 0) {
                index++;
                if (index < select_stmt->inner_join_filter_stmts().size() && select_stmt->inner_join_filter_stmts().at(index) != nullptr) {
                    /* code */
                    unique_ptr<LogicalOperator> on_predicate_oper;
                    RC rc = create_plan(select_stmt->inner_join_filter_stmts().at(index), on_predicate_oper);
                    on_predicate_oper->add_child(std::move(unique_ptr<LogicalOperator>(join_oper)));
                    table_oper = std::move(on_predicate_oper);
                } else {
                    table_oper = unique_ptr<LogicalOperator>(join_oper);
                }
            } else {
                table_oper = unique_ptr<LogicalOperator>(join_oper);
            }
        }
    }
    unique_ptr<LogicalOperator>* top_op = &table_oper;
    unique_ptr<LogicalOperator> predicate_oper;
    RC rc = create_plan(select_stmt->filter_stmt(), predicate_oper);
    if (rc != RC::SUCCESS) {
        LOG_WARN("failed to create predicate logical plan. rc=%s", strrc(rc));
        return rc;
    }
    if (predicate_oper) {
        predicate_oper->add_child(std::move(*top_op));
        top_op = &predicate_oper;
    }

    // sortby_4_groupby
    unique_ptr<LogicalOperator> sorted_for_groupby;
    // 如果是聚合函数(不包含groupby)，则不会生成sorted_4_groupby算子
    if (select_stmt->orderby_for_groupby()) {
        rc = create_plan(select_stmt->orderby_for_groupby(), sorted_for_groupby);
        if (rc != RC::SUCCESS) {
            LOG_WARN("failed to create sorted for groupby. rc=%s", strrc(rc));
            return rc;
        }
    }
    if (sorted_for_groupby) {
        sorted_for_groupby->add_child(std::move(*top_op));
        top_op = &sorted_for_groupby;
    }
    // groupby 算子创建
    std::vector<AggrFuncExpr*> aggr_exprs;
    std::vector<FieldExpr*> field_exprs;
    const std::vector<Expression*>& exprs = select_stmt->query_expressions();
    for (const auto& expr : exprs) {
        switch (expr->type()) {
            case ExprType::AGGRFUNCTION: {
                aggr_exprs.emplace_back((AggrFuncExpr*)expr);
                break;
            }
            case ExprType::FIELD: {
                field_exprs.emplace_back((FieldExpr*)expr);
                break;
            }
            case ExprType::ARITHMETIC: {
                ArithmeticExpr::find_aggr_func((ArithmeticExpr*)expr, aggr_exprs);
                break;
            }
        }
    }

    unique_ptr<LogicalOperator> groupby_oper;
    const GroupByStmt* groupby_stmt = select_stmt->groupby_stmt();
    // 没有groupby语句却查询了聚合函数和非聚合函数列
    if (!select_stmt->orderby_for_groupby() && !aggr_exprs.empty() && !field_exprs.empty()) {
        return RC::SQL_SYNTAX;
    }
    if (groupby_stmt != nullptr && !groupby_stmt->groupby_units().empty()) {
        if (field_exprs.empty()) {
            return RC::SQL_SYNTAX;
        }
        // 检查field是否包含全部的GroupBy依赖的字段
        for (const auto* field_expr : field_exprs) {
            bool in_group = false;
            for (const auto* unit : groupby_stmt->groupby_units()) {
                const Expression* expr = unit->expr();
                if (expr->type() != ExprType::FIELD) {
                    LOG_ERROR("groupby unit's type != FieldExpr");
                    return RC::SQL_SYNTAX;
                }
                if (((FieldExpr*)expr)->field() == ((FieldExpr*)field_expr)->field()) {
                    in_group = true;
                    break;
                }
            }
            if (!in_group) {
                printf("%s can't find a match select field", field_expr->field_name());
                LOG_ERROR("%s can't find a match select field", field_expr->field_name());
                return RC::SQL_SYNTAX;
            }
        }
    }
    if ((groupby_stmt != nullptr && !groupby_stmt->groupby_units().empty()) || !aggr_exprs.empty()) {
        rc = create_plan(select_stmt->groupby_stmt(), groupby_oper);
        if (rc != RC::SUCCESS) {
            LOG_WARN("fail to create groupby logical plan. rc=%s", strrc(rc));
            return rc;
        }
    }
    if (groupby_oper) {
        GroupByLogicalOperator* oper = (GroupByLogicalOperator*)groupby_oper.get();
        oper->aggr_exprs().swap(aggr_exprs);
        oper->field_exprs().swap(field_exprs);
        groupby_oper->add_child(std::move(*top_op));
        top_op = &groupby_oper;
    }

    // having 算子

    // sortby 算子
    unique_ptr<LogicalOperator> sorted_logical_oper;
    // TODO 封装到create_plan
    if (select_stmt->orderby_stmt() != nullptr && !select_stmt->orderby_stmt()->orderby_units().empty()) {
        sorted_logical_oper.reset(new SortedLogicalOperator(&select_stmt->orderby_stmt()->orderby_units()));
    }
    if (sorted_logical_oper) {
        sorted_logical_oper->add_child(std::move(*top_op));
        top_op = &sorted_logical_oper;
    }

    // 投影算子
    unique_ptr<LogicalOperator> project_oper(new ProjectLogicalOperator(&expressions));
    project_oper->add_child(std::move(*top_op));

    logical_operator.swap(project_oper);
    return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    FilterStmt *filter_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  std::vector<unique_ptr<Expression>> cmp_exprs;
  const std::vector<FilterUnit *> &filter_units = filter_stmt->filter_units();

  auto handle_subquery = [&](const FilterObj &filter_obj) -> RC {
    if (filter_obj.is_attr && filter_obj.expression->type() == ExprType::SUBQUERY) {
      SubQueryExpr *expr = static_cast<SubQueryExpr *>(filter_obj.expression);
      std::unique_ptr<LogicalOperator> sub_query;
      RC rc = create_plan(expr->select_stmt(), sub_query);
      if (rc != RC::SUCCESS) {
        return rc;
      }
      expr->set_sub_query_logical(static_cast<ProjectLogicalOperator *>(sub_query.get()));
      // 释放智能指针对申请的内存的管理
      sub_query.release();
      return rc;
    }
    return RC::SUCCESS;
  };

  for (const FilterUnit *filter_unit : filter_units) {
    const FilterObj &filter_obj_left = filter_unit->left();
    const FilterObj &filter_obj_right = filter_unit->right();

    unique_ptr<Expression> left(filter_obj_left.is_attr
                                         ? static_cast<Expression *>(filter_obj_left.expression)
                                         : static_cast<Expression *>(new ValueExpr(filter_obj_left.value)));
    RC rc = handle_subquery(filter_obj_left);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    unique_ptr<Expression> right(filter_obj_right.is_attr
                                          ? static_cast<Expression *>(filter_obj_right.expression)
                                          : static_cast<Expression *>(new ValueExpr(filter_obj_right.value)));
    handle_subquery(filter_obj_right);
    if (rc != RC::SUCCESS) {
      return rc;
    }

        ComparisonExpr* cmp_expr = new ComparisonExpr(filter_unit->comp(), std::move(left), std::move(right));
        cmp_exprs.emplace_back(cmp_expr);
    }

    unique_ptr<PredicateLogicalOperator> predicate_oper;
    if (!cmp_exprs.empty()) {
        unique_ptr<ConjunctionExpr> conjunction_expr(new ConjunctionExpr(ConjunctionExpr::Type::AND, cmp_exprs));
        predicate_oper = unique_ptr<PredicateLogicalOperator>(new PredicateLogicalOperator(std::move(conjunction_expr)));
    }

    logical_operator = std::move(predicate_oper);
    return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    OrderByStmt* orderby_stmt,
    std::unique_ptr<LogicalOperator>& logical_operator) {
    if (orderby_stmt) {
        SortedLogicalOperator* orderby = new SortedLogicalOperator(&orderby_stmt->orderby_units());
        logical_operator.reset(orderby);
    }
    return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    GroupByStmt* groupby_stmt,
    std::unique_ptr<LogicalOperator>& logical_operator) {
    if (groupby_stmt) {
        GroupByLogicalOperator* groupby = new GroupByLogicalOperator(groupby_stmt);
        logical_operator.reset(groupby);
    }
    return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    InsertStmt* insert_stmt,
    unique_ptr<LogicalOperator>& logical_operator) {
    Table* table = insert_stmt->table();
    const vector<vector<Value>>* insert_values = insert_stmt->values();

    InsertLogicalOperator* insert_operator = new InsertLogicalOperator(table, insert_values);
    logical_operator.reset(insert_operator);
    return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(UpdateStmt* update_stmt, std::unique_ptr<LogicalOperator>& logical_operator) {
    Table* table = update_stmt->table();
    int count = update_stmt->value_amount();

    // 找到要修改的字段来生成执行计划
    vector<Field> fields;
    for (const auto& pair : update_stmt->update_map()) {
        const std::string& attribute_name = pair.first;
        Value* value = pair.second; 
        const FieldMeta* field_meta = table->table_meta().field(attribute_name.c_str());
        fields.push_back(Field(table, field_meta));
    }
    unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, fields, false));

    // 过滤执行计划
    unique_ptr<LogicalOperator> predicate_oper;
    RC rc = create_plan(update_stmt->filter_stmt(), predicate_oper);
    if (rc != RC::SUCCESS) {
        return rc;
    }
    unique_ptr<LogicalOperator> update_oper(new UpdateLogicalOperator(table, update_stmt->update_map()));
    if (predicate_oper) {
        predicate_oper->add_child(std::move(table_get_oper));
        update_oper->add_child(std::move(predicate_oper));
    } else {
        update_oper->add_child(std::move(table_get_oper));
    }
    logical_operator = std::move(update_oper);
    return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(
    DeleteStmt* delete_stmt,
    unique_ptr<LogicalOperator>& logical_operator) {
    Table* table = delete_stmt->table();
    FilterStmt* filter_stmt = delete_stmt->filter_stmt();
    std::vector<Field> fields;
    for (int i = table->table_meta().sys_field_num(); i < table->table_meta().field_num(); i++) {
        const FieldMeta* field_meta = table->table_meta().field(i);
        fields.push_back(Field(table, field_meta));
    }
    unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, fields, false /*readonly*/));

    unique_ptr<LogicalOperator> predicate_oper;
    RC rc = create_plan(filter_stmt, predicate_oper);
    if (rc != RC::SUCCESS) {
        return rc;
    }

    unique_ptr<LogicalOperator> delete_oper(new DeleteLogicalOperator(table));

    if (predicate_oper) {
        predicate_oper->add_child(std::move(table_get_oper));
        delete_oper->add_child(std::move(predicate_oper));
    } else {
        delete_oper->add_child(std::move(table_get_oper));
    }

    logical_operator = std::move(delete_oper);
    return rc;
}

RC LogicalPlanGenerator::create_plan(
    ExplainStmt* explain_stmt,
    unique_ptr<LogicalOperator>& logical_operator) {
    Stmt* child_stmt = explain_stmt->child();
    unique_ptr<LogicalOperator> child_oper;
    RC rc = create(child_stmt, child_oper);
    if (rc != RC::SUCCESS) {
        LOG_WARN("failed to create explain's child operator. rc=%s", strrc(rc));
        return rc;
    }

    logical_operator = unique_ptr<LogicalOperator>(new ExplainLogicalOperator);
    logical_operator->add_child(std::move(child_oper));
    return rc;
}
