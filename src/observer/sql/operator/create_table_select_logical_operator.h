#pragma once

#include "sql/operator/logical_operator.h"

class CreateTableSelectLogicalOperator : public LogicalOperator {
public:
  LogicalOperatorType type() const override {
    return LogicalOperatorType::CREATE_TABLE_SELECT;
  }
};
