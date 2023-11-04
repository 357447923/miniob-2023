#pragma once

#include "sql/operator/logical_operator.h"
#include "storage/field/field.h"

class Table;

typedef Table View;

class ViewGetLogicalOperator : public LogicalOperator {
public:
  ViewGetLogicalOperator(View *view, const std::vector<Field> &fields, bool readonly);

  ~ViewGetLogicalOperator() = default;

  LogicalOperatorType type() const override {
    return LogicalOperatorType::VIEW_GET;
  }

  const View *view() const {
    return view_;
  }
  const std::vector<Field> fields() const {
    return fields_;
  }
  const std::vector<Table *> attach_to() const {
    return attach_to_;
  }
  bool readonly() const { 
    return readonly_; 
  }

private:
  bool readonly_ = false;
  View *view_ = nullptr;
  std::vector<Field> fields_;
  const std::vector<Table *> attach_to_;
};