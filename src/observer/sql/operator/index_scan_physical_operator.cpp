/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022/07/08.
//

#include "sql/operator/index_scan_physical_operator.h"
#include "storage/index/index.h"
#include "storage/trx/trx.h"
#include "event/sql_debug.h"
IndexScanPhysicalOperator::IndexScanPhysicalOperator(
    Table *table, Index *index, bool readonly, 
    const Value *left_value, bool left_inclusive, 
    const Value *right_value, bool right_inclusive)
    : table_(table), 
      index_(index), 
      readonly_(readonly), 
      left_inclusive_(left_inclusive), 
      right_inclusive_(right_inclusive)
{
  if (left_value) {
    left_value_ = *left_value;
  }
  if (right_value) {
    right_value_ = *right_value;
  }
}

RC IndexScanPhysicalOperator::open(Trx *trx)
{
  if (nullptr == table_ || nullptr == index_) {
    return RC::INTERNAL;
  }
  int value_len = 0;
  for (int i = 1; i < index_->index_meta().fields()->size(); i++) {
    const char *field_name = index_->index_meta().fields()->at(i).c_str();
    value_len += table_->table_meta().field(field_name)->len();
  }  
  // 给左右值加上Bitmap
  int map_len = table_->table_meta().field(0)->offset();
  char *left_with_bitmap = new char[map_len + value_len];
  char *right_with_bitmap = new char[map_len + value_len];
  memset(left_with_bitmap, 0, map_len + value_len);  
  memset(right_with_bitmap, 0, map_len + value_len);  
  common::Bitmap left_map(left_with_bitmap, map_len);
  common::Bitmap right_map(right_with_bitmap, map_len);
  if (left_value_.attr_type() == NULLS)
  {
      const char *field_name = index_->index_meta().fields()->at(1).c_str();
      int field_id = table_->table_meta().field(field_name)->id();
      left_map.set_bit(field_id);
  }
  if (right_value_.attr_type() == NULLS)
  {
      const char *field_name = index_->index_meta().fields()->at(1).c_str();
      int field_id = table_->table_meta().field(field_name)->id();
      right_map.set_bit(field_id);
  }
  memcpy(left_with_bitmap + map_len, left_value_.data(), value_len);
  memcpy(right_with_bitmap + map_len, right_value_.data(), value_len);

  LOG_INFO("use index:%s",index_->index_meta().name());
  IndexScanner *index_scanner = index_->create_scanner(
      left_with_bitmap,
      map_len + left_value_.length(),
      left_inclusive_,
      right_with_bitmap,
      map_len + right_value_.length(),
      right_inclusive_);
  delete[] left_with_bitmap;
  delete[] right_with_bitmap;
  if (nullptr == index_scanner) {
    LOG_WARN("failed to create index scanner");
    return RC::INTERNAL;
  }

  record_handler_ = table_->record_handler();
  if (nullptr == record_handler_) {
    LOG_WARN("invalid record handler");
    index_scanner->destroy();
    return RC::INTERNAL;
  }
  index_scanner_ = index_scanner;

  tuple_.set_schema(table_, table_->table_meta().field_metas());

  trx_ = trx;
  return RC::SUCCESS;
}

RC IndexScanPhysicalOperator::next()
{
  RID rid;
  RC rc = RC::SUCCESS;

  record_page_handler_.cleanup();

  bool filter_result = false;
  bool idx_need_increase = get_idx_need_increase();
  while (RC::SUCCESS == (rc = index_scanner_->next_entry(&rid, idx_need_increase))) {
    rc = record_handler_->get_record(record_page_handler_, &rid, readonly_, &current_record_);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    tuple_.set_record(&current_record_);
    rc = filter(tuple_, filter_result);
    if (rc != RC::SUCCESS) {
      return rc;
    }

    if (!filter_result) {
      continue;
    }

    rc = trx_->visit_record(table_, current_record_, readonly_);
    if (rc == RC::RECORD_INVISIBLE) {
      continue;
    } else {
      return rc;
    }
  }

  return rc;
}

RC IndexScanPhysicalOperator::close()
{
  index_scanner_->destroy();
  index_scanner_ = nullptr;
  return RC::SUCCESS;
}

Tuple *IndexScanPhysicalOperator::current_tuple()
{
  tuple_.set_record(&current_record_);
  return &tuple_;
}

void IndexScanPhysicalOperator::set_predicates(std::vector<std::unique_ptr<Expression>> &&exprs)
{
  predicates_ = std::move(exprs);
}

RC IndexScanPhysicalOperator::filter(RowTuple &tuple, bool &result)
{
  RC rc = RC::SUCCESS;
  Value value;
  for (std::unique_ptr<Expression> &expr : predicates_) {
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

void IndexScanPhysicalOperator::set_idx_need_increase(bool idx_need_increase) {
    idx_need_increase_ = idx_need_increase;
}

const bool IndexScanPhysicalOperator::get_idx_need_increase() {
    return idx_need_increase_;
}

std::string IndexScanPhysicalOperator::param() const
{
  return std::string(index_->index_meta().name()) + " ON " + table_->name();
}
