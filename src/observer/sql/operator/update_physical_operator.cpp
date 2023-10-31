#include "sql/operator/update_physical_operator.h"
#include "storage/trx/trx.h"
#include "storage/record/record.h"
#include "storage/index/index.h"
#include "event/sql_debug.h"

UpdatePhysicalOperator::UpdatePhysicalOperator(Table *table, std::unordered_map<std::string, Value*> update_map) {
  table_ = table;
  update_map_= std::move(update_map);
}

UpdatePhysicalOperator::~UpdatePhysicalOperator() = default;

RC UpdatePhysicalOperator::open(Trx *trx) {
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  std::unique_ptr<PhysicalOperator> &child = children_[0];
  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }

  trx_ = trx;

  return RC::SUCCESS;
}

RC UpdatePhysicalOperator::close() {
    if (!children_.empty()) {
        children_[0]->close();
    }
    return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next() {
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    return RC::RECORD_EOF;
  }
  PhysicalOperator *child = children_[0].get();
  while (RC::SUCCESS == (rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }
    
    RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Record &record = row_tuple->record();
    int index;
    // std::vector<int> offsets, std::vector<int> indexs, std::vector<Value&> values
    std::vector<int> offsets;
    std::vector<int> indexs;
    std::vector<Value> values;
    for (const auto& pair : update_map_) {
      const std::string& field_name = pair.first;
      Value* value = pair.second; 
      const FieldMeta *field = table_->table_meta().field(field_name.c_str(), index);
      int offset = field->offset();
      offsets.push_back(offset);
      indexs.push_back(index);
      values.push_back(std::move(*value));
    }
    rc = trx_->update_record(table_, record, std::move(offsets), std::move(indexs), std::move(values));
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to update record: %s", strrc(rc));
      sql_debug("failed to update record: %s", strrc(rc));
      sql_debug("test");
      return rc;
    }
  }
  table_->delete_cache_record();
  return RC::RECORD_EOF;
}