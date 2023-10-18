#include "sql/operator/sorted_physical_operator.h"

SortedPhysicalOperator::~SortedPhysicalOperator() {
  for (auto &record : records_) {
    for (auto rec : record) {
      delete rec;
    }
  }
  records_.clear();
}

RC SortedPhysicalOperator::fetch_and_sort_table() {
  RC rc = RC::SUCCESS;

  int index = 0;
  typedef std::pair<std::vector<Value>, int> CmpPair;
  std::vector<CmpPair> pair_sort_table;
  std::vector<Value> pair_cell;
  while (RC::SUCCESS == (rc = children_[0]->next())) {
    pair_cell.clear();
    for (const OrderByUnit *unit : *units_) {
      Expression *expr = unit->expr();
      Value cell;
      expr->get_value(*children_[0]->current_tuple(), cell);
      pair_cell.emplace_back(cell);
    }
    pair_sort_table.emplace_back(std::make_pair(pair_cell, index++));
    
    std::vector<Record *> records;
    children_[0]->current_tuple()->get_record(records);
    for (auto &rcd_ptr : records) {
      rcd_ptr = new Record(*rcd_ptr);
    }
    records_.emplace_back(records);
  }
  if (RC::RECORD_EOF != rc) {
    LOG_ERROR("Fetch Table Error In SortedPhysicalOperator. RC: %d", rc);
    return rc;
  }
  if (records_.empty()) {
    return RC::RECORD_EOF;
  }
  rc = RC::SUCCESS;
  LOG_INFO("Fetch Table Success In SortedPhysicalOperator");

  bool order[units_->size()];
  for (int i = 0; i < units_->size(); ++i) {
    order[i] = (*units_)[i]->sort_type();
  }
  auto cmp = [&order](const CmpPair &a, const CmpPair &b) {
    auto &cells_a = a.first;
    auto &cells_b = b.first;
    assert(cells_a.size() == cells_b.size());
    for (size_t i = 0; i < cells_a.size(); i++) {
      auto &cell_a = cells_a[i];
      auto &cell_b = cells_b[i];
      if (cell_a.is_null() && cell_b.is_null()) {
        continue;
      }
      if (cell_a.is_null()) {
        return order[i]? true: false;
      }
      if (cell_b.is_null()) {
        return order[i]? true: false;
      }
      if (cell_a != cell_b) {
        return order[i]? (cell_a.compare(cell_b) == RC::LEFT_LT_ANOTHER)
            :(cell_a.compare(cell_b) == RC::LEFT_GT_ANOTHER);
      }
    }
    return false;
  };
  std::sort(pair_sort_table.begin(), pair_sort_table.end(), cmp);
  LOG_INFO("Sort Table Success");

  for (int i = 0; i < pair_sort_table.size(); ++i) {
    ordered_idx_.emplace_back(pair_sort_table[i].second);
  }
  it_ = ordered_idx_.begin();
  return rc;
}

RC SortedPhysicalOperator::open(Trx *trx) {
  if (RC::SUCCESS != children_[0]->open(trx)) {
    LOG_WARN("SortedPhysicalOperator child open failed!");
    return RC::INTERNAL;
  }
  return RC::SUCCESS;
}

RC SortedPhysicalOperator::next() {
  RC rc = RC::SUCCESS;
  if (is_first) {
    is_first = false;
    rc = fetch_and_sort_table();
    return rc;
  }
  
  it_++;
  if (ordered_idx_.end() != it_) {
    return RC::SUCCESS;
  }

  return RC::RECORD_EOF;
}

RC SortedPhysicalOperator::close() {
  return children_[0]->close();
}

Tuple *SortedPhysicalOperator::current_tuple() {
  // TODO 需要改动整个set和get_record的抽象
  Tuple *tuple = children_[0]->current_tuple();
  if (is_first) {
    return tuple;
  }
  if (it_ != ordered_idx_.end()) {
    tuple->set_record((records_)[*it_][0]);
  }
  return tuple;
}