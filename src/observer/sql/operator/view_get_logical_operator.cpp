#include "sql/operator/view_get_logical_operator.h"

ViewGetLogicalOperator::ViewGetLogicalOperator(View *view, const std::vector<Field> &fields, bool readonly)
  : view_(view), fields_(fields), readonly_(readonly) {}