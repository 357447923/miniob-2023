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
// Created by Longda on 2021/4/13.
//

#include <string>
#include <sstream>

#include "sql/executor/execute_stage.h"

#include "common/log/log.h"
#include "session/session.h"
#include "event/storage_event.h"
#include "event/sql_event.h"
#include "event/session_event.h"
#include "sql/stmt/stmt.h"
#include "sql/stmt/select_stmt.h"
#include "storage/default/default_handler.h"
#include "sql/executor/command_executor.h"
#include "sql/operator/calc_physical_operator.h"

using namespace std;
using namespace common;

RC ExecuteStage::handle_request(SQLStageEvent *sql_event)
{
  RC rc = RC::SUCCESS;
  const unique_ptr<PhysicalOperator> &physical_operator = sql_event->physical_operator();
  if (physical_operator != nullptr) {
    return handle_request_with_physical_operator(sql_event);
  }

  SessionEvent *session_event = sql_event->session_event();

  Stmt *stmt = sql_event->stmt();
  if (stmt != nullptr) {
    CommandExecutor command_executor;
    rc = command_executor.execute(sql_event);
    session_event->sql_result()->set_return_code(rc);
  } else {
    return RC::INTERNAL;
  }
  return rc;
}

static inline void append_aggr_schema(TupleSchema &tuple_schema, bool with_table_name, AggrFuncExpr *expr) {
  FieldExpr *field = expr->field();
  if (field) {
    if (with_table_name) {
      tuple_schema.append_cell(TupleCellSpec(field->table_name(), field->field_name(), expr->alias(), expr->aggr_type()));
    }else {
      tuple_schema.append_cell(TupleCellSpec(nullptr, field->field_name(), expr->alias(), expr->aggr_type()));
    }
  }else {
    Value &value = const_cast<Value &>(expr->value()->get_value());
    if (expr->alias() != nullptr) {
      tuple_schema.append_cell(TupleCellSpec(expr->alias(), expr->aggr_type()));
      return;
    }
    std::string display;
    display.append(AGGR_FUNC_TYPE_NAME[expr->aggr_type()]).append("(").append(value.data()).append(")");
    value.set_string(display.c_str(), display.size());
    tuple_schema.append_cell(TupleCellSpec(value.data(), expr->aggr_type()));
  }
}

RC ExecuteStage::handle_request_with_physical_operator(SQLStageEvent *sql_event)
{
  RC rc = RC::SUCCESS;

  Stmt *stmt = sql_event->stmt();
  ASSERT(stmt != nullptr, "SQL Statement shouldn't be empty!");

  unique_ptr<PhysicalOperator> &physical_operator = sql_event->physical_operator();
  ASSERT(physical_operator != nullptr, "physical operator should not be null");

  // TODO 这里也可以优化一下，是否可以让physical operator自己设置tuple schema
  TupleSchema schema;
  switch (stmt->type()) {
    case StmtType::SELECT: {
      SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);
      // 不一定是多表才会带着表名
      // 但miniob可以这样认为
      bool with_table_name = select_stmt->tables().size() > 1;
      const std::vector<Expression *> &expressions = select_stmt->query_expressions();
    
      for (int i = 0; i < expressions.size(); i++) {
        switch(expressions[i]->type()) {
          case ExprType::AGGRFUNCTION: {
            AggrFuncExpr *aggr_expr = static_cast<AggrFuncExpr *>(expressions[i]);
            append_aggr_schema(schema, with_table_name, aggr_expr);
          }break;
          case ExprType::FIELD: {
            FieldExpr *field_expr = static_cast<FieldExpr *>(expressions[i]);
            if (with_table_name) {
              schema.append_cell(TupleCellSpec(field_expr->table_name(), 
                  field_expr->field_name(), field_expr->alias()));
            }else {
              schema.append_cell(TupleCellSpec(field_expr->field_name(), 
                  field_expr->alias()));
            }
          }break;
          case ExprType::ARITHMETIC: {
            const char *alias = expressions[i]->alias();
            schema.append_cell(alias? alias: expressions[i]->name().c_str());
          }break;
          case ExprType::FUNC: {
            FuncExpr *func_expr = static_cast<FuncExpr *>(expressions[i]);
            schema.append_cell(TupleCellSpec(func_expr->func_type(), func_expr, func_expr->name().c_str()));
          }break;
          default: {
            LOG_WARN("Execute_stage couldn't handle expr_type => %d", expressions[i]->type());
          }break;
        }
      }
    } break;

    case StmtType::CALC: {
      CalcPhysicalOperator *calc_operator = static_cast<CalcPhysicalOperator *>(physical_operator.get());
      for (const unique_ptr<Expression> & expr : calc_operator->expressions()) {
        const char *alias = expr->alias();
        schema.append_cell(alias? alias: expr->name().c_str());
      }
    } break;

    case StmtType::EXPLAIN: {
      schema.append_cell("Query Plan");
    } break;
    default: {
      // 只有select返回结果
    } break;
  }

  SqlResult *sql_result = sql_event->session_event()->sql_result();
  sql_result->set_tuple_schema(schema);
  sql_result->set_operator(std::move(physical_operator));
  return rc;
}
