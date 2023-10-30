#include "sql/operator/create_table_select_physical_operator.h"

RC CreateTableSelectPhysicalOperator::open(Trx *trx) {
  assert(children_.size() == 1);
  
  return children_[0]->open(trx);
}

RC CreateTableSelectPhysicalOperator::next() {
  return children_[0]->next();
}

RC CreateTableSelectPhysicalOperator::close() {
  return children_[0]->close();
}

Tuple *CreateTableSelectPhysicalOperator::current_tuple() {
  return children_[0]->current_tuple();
}