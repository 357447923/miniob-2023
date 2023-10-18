#include "sql/operator/sorted_logical_operator.h"

SortedLogicalOperator::SortedLogicalOperator(const std::vector<OrderByUnit *> *units) : units_(units){}

