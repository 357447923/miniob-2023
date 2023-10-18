#pragma once

#include <vector>
#include "sql/operator/logical_operator.h"
#include "sql/stmt/group_by_stmt.h"
#include "sql/parser/parse_defs.h"

/**
 * @brief GroupBy算子
 * @ingroup LogicalOperator
 * @details 从表中获取数据后，进行分组
 */
class GroupByLogicalOperator : public LogicalOperator {
public:
  GroupByLogicalOperator(const GroupByStmt *stmt) : stmt_(stmt){}

  LogicalOperatorType type() const override
  {
    return LogicalOperatorType::GROUP;
  }

  const GroupByStmt *groupby_stmt() const {
    return stmt_;
  }

  std::vector<AggrFuncExpr *> &aggr_exprs() {
    return aggr_exprs_;
  }

  std::vector<FieldExpr *> &field_exprs() {
    return field_exprs_;
  }
private:
  std::vector<AggrFuncExpr *> aggr_exprs_;
  std::vector<FieldExpr *> field_exprs_;
  const GroupByStmt *stmt_;
};