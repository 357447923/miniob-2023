#ifndef MINIOB_UPDATE_PHYSICAL_OPERATOR_H
#define MINIOB_UPDATE_PHYSICAL_OPERATOR_H

#include "sql/operator/physical_operator.h"
#include "sql/parser/parse.h"

class UpdatePhysicalOperator : public PhysicalOperator {
public:

  UpdatePhysicalOperator(Table *table, std::unordered_map<std::string, Value*>);
  
  virtual ~UpdatePhysicalOperator() override;

  PhysicalOperatorType type() const override {
      return PhysicalOperatorType::UPDATE;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override { return nullptr; }

private:
  Table * table_ = nullptr;
  // Value value_;
  // const char * field_name_ = nullptr;
  std::unordered_map<std::string, Value*> update_map_ ;
  Trx *trx_ = nullptr;
};

#endif