#include "sql/operator/view_scan_physical_operator.h"
#include "storage/table/table.h"

std::string ViewScanPhysicalOperator::param() const { return view_->name(); }

RC ViewScanPhysicalOperator::open(Trx *trx) {
  assert(view_->project_physical_oper_ != nullptr);
  return view_->project_physical_oper_->open(trx);
}
RC ViewScanPhysicalOperator::next() {
  RC rc = RC::SUCCESS;
  bool filter_result = false;
  while ((rc = view_->project_physical_oper_->next()) == RC::SUCCESS) {
    filter(*current_tuple(), filter_result);
    if (filter_result) {
      break;
    }
  }
  return rc;
}
RC ViewScanPhysicalOperator::close() {
  return view_->project_physical_oper_->close();
}
Tuple *ViewScanPhysicalOperator::current_tuple() {
  Tuple *tuple = view_->project_physical_oper_->current_tuple();
  tuple_.set_tuple(tuple);
  return &tuple_;
}
void ViewScanPhysicalOperator::set_predicates(std::vector<std::unique_ptr<Expression>> &&exprs) {
  predicates_.swap(exprs);
}

RC ViewScanPhysicalOperator::filter(Tuple &tuple, bool &result) {
  RC rc = RC::SUCCESS;
  
  Value value;
  for (unique_ptr<Expression> &expr : predicates_) {
    rc = expr->get_value(tuple, value);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    bool tmp_result = value.get_boolean();
    if (!tmp_result) {
      result = false;
      return rc;
    }
  }

  result = true;
  return rc;
}