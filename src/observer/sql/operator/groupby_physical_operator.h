#pragma once

#include "sql/operator/physical_operator.h"
#include "sql/expr/tuple.h"
#include "sql/stmt/group_by_stmt.h"

class Trx;

class GroupByPhysicalOperator : public PhysicalOperator {
public:
  GroupByPhysicalOperator(const std::vector<GroupByUnit *> *units, const std::vector<AggrFuncExpr *> &aggr_exprs,
      const std::vector<FieldExpr *> &field_exprs);

  ~GroupByPhysicalOperator() = default;

  PhysicalOperatorType type() const override {
    return PhysicalOperatorType::GROUP;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override; 

private:
  bool is_first_ = true;
  bool is_new_group_ = true;
  bool is_record_eof_ = false;

  const std::vector<GroupByUnit *> *units_ = nullptr;
  std::vector<Value> pre_group_val_;    // 原本组的数据
  GroupTuple tuple_;
};

