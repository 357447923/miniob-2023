#pragma once

#include "sql/operator/physical_operator.h"
#include "storage/record/record_manager.h"
#include "common/rc.h"

class Table;

typedef Table View;

class ViewScanPhysicalOperator : public PhysicalOperator
{
public:
  ViewScanPhysicalOperator(View *view, bool readonly) 
      : view_(view), readonly_(readonly), tuple_(ViewTuple(view))
  {}

  virtual ~ViewScanPhysicalOperator() = default;

  std::string param() const override;

  PhysicalOperatorType type() const override
  {
    return PhysicalOperatorType::VIEW_SCAN;
  }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;

  void set_predicates(std::vector<std::unique_ptr<Expression>> &&exprs);

private:
  RC filter(Tuple &tuple, bool &result);

public:
  int                                      get_record_count_ = 0;
private:
  View *                                   view_ = nullptr;
  Trx *                                    trx_ = nullptr;
  bool                                     readonly_ = false;
  ViewTuple                                tuple_;
  std::vector<std::unique_ptr<Expression>> predicates_; // TODO chang predicate to table tuple filter
};