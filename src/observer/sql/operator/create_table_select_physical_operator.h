#pragma once

#include "sql/operator/physical_operator.h"
#include "sql/executor/create_table_executor.h"

class CreateTableSelectPhysicalOperator : public PhysicalOperator {
public:
  PhysicalOperatorType type() const override {
    return PhysicalOperatorType::CREATE_TABLE;
  }

  virtual RC open(Trx *trx) override;
  virtual RC next() override;
  virtual RC close() override;

  virtual Tuple *current_tuple() override;

  

private:
  CreateTableExecutor executor;
};