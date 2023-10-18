#pragma once

#include <vector>
#include "sql/operator/logical_operator.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/orderby_stmt.h"

class SortedLogicalOperator : public LogicalOperator {
public:
  SortedLogicalOperator(const std::vector<OrderByUnit *> *units);

  LogicalOperatorType type() const override
  {
    return LogicalOperatorType::SORTED;
  }

  const std::vector<OrderByUnit *> *units() const {
    return units_;
  }

private:
  const std::vector<OrderByUnit *> *units_;
};