#include "sql/operator/create_table_select_physical_operator.h"
#include "sql/stmt/create_table_stmt.h"

RC CreateTableSelectPhysicalOperator::open(Trx *trx) {
  assert(children_.size() == 1);
  assert(db_ != nullptr);
  assert(create_table_stmt_ != nullptr && create_table_stmt_->select_stmt() != nullptr);
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