#include "sql/operator/update_physical_operator.h"
#include "storage/trx/trx.h"
#include "storage/record/record.h"
#include "storage/index/index.h"
#include "event/sql_debug.h"
#include "common/typecast.h"

UpdatePhysicalOperator::UpdatePhysicalOperator(Table *table, std::unordered_map<std::string, Expression*> update_map) {
  table_ = table;
  update_map_= std::move(update_map);
}

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
    std::vector<int> lens;
    std::vector<int> indexs;
    std::vector<Value> values;
    for (const auto& pair : update_map_) {
      const std::string& field_name = pair.first;
      Expression* expr = pair.second; 
      const FieldMeta *field = table_->table_meta().field(field_name.c_str(), index);
      int offset = field->offset();
      int len = field->len();
      offsets.push_back(offset);
      lens.push_back(len);
      indexs.push_back(index);
      // 获取值，并且做类型检查, 子查询条目数检查
      Value value;
      if (expr->type() == ExprType::SUBQUERY) {
        SubQueryExpr *sub_query = static_cast<SubQueryExpr *>(expr);
        sub_query->set_trx(trx_);
        sub_query->open_sub_query();
        rc = expr->get_value(*tuple, value);
        if (rc != RC::SUCCESS) {
          LOG_INFO("get value fail.%s\t %d", __FILE__, __LINE__);
          sub_query->close_sub_query();
          return RC::INTERNAL;
        }
        Value tmp;
        rc = expr->get_value(*tuple, tmp);
        if (rc != RC::RECORD_EOF) {
          if (rc == RC::SUCCESS) {
            LOG_INFO("update can not receive mutiple value");
          }else {
            LOG_INFO("execption happen. file: %s, line: %d", __FILE__, __LINE__);
          }
          sub_query->close_sub_query();
          return RC::INTERNAL;
        }
        sub_query->close_sub_query();
        common::type_cast(value, field->type());
      }else {
        rc = expr->get_value(*tuple, value);
        if (rc != RC::SUCCESS) {
          LOG_INFO("get value fail.%s\t %d", __FILE__, __LINE__);
          return RC::INTERNAL;
        }
      }

      AttrType value_type = value.attr_type();
      if (value_type != field->type() && !(value_type == NULLS && !field->not_null())) {
          LOG_INFO("value attr_type: %d, expect: %d", field->type());
          return RC::INTERNAL;
      }
      values.push_back(std::move(value));
    }
    rc = trx_->update_record(table_, record, std::move(offsets), std::move(indexs), std::move(values), std::move(lens));
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