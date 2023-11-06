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
// Created by WangYunlai on 2021/6/9.
//

#include "sql/operator/insert_physical_operator.h"
#include "sql/stmt/insert_stmt.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

using namespace std;

InsertPhysicalOperator::InsertPhysicalOperator(Table *table, const vector<vector<Value>> *values)
    : table_(table), values_(values)
{}

RC InsertPhysicalOperator::open(Trx *trx)
{
  vector<Record> records;
  int            count = static_cast<int>(values_->size());
  if (table_->project_physical_oper_ != nullptr) {
    const std::vector<FieldMeta>                            &field_metas = *table_->table_meta().field_metas();
    std::unordered_map<const Table *, vector<vector<Value>>> values_map;
    std::unordered_map<const Table *, vector<int>>           loc_map;
    for (int i = 0; i < (*values_)[0].size(); i++) {
      const FieldMeta &view_field_meta = field_metas[i];
      const Table     *origin_table    = table_->field_link_to_table_[&view_field_meta];
      if (origin_table == nullptr)
      {
        return RC::INTERNAL;
      }
      loc_map[origin_table].push_back(i);
    }

    for (const auto& pair : loc_map) {
      const Table* ori_table = pair.first;
      const std::vector<FieldMeta> *ori_fields = ori_table->table_meta().field_metas();
      vector<Value> one_table_one_record;
      const vector<int> &loc = pair.second;
      for (int i = 0; i < ori_fields->size(); i++) {
        int offset = -1;
        for (int j = 0; j < loc.size(); j++) {
          if (&(*ori_fields)[i] == table_->view_field_link_to_origin_field_[&field_metas[loc[j]]]) {
            offset = loc[j];
          }
        }
        if (offset == -1) {
          const FieldMeta &ori_field = ori_fields->at(i);
          if (ori_field.not_null()) {
            return RC::INTERNAL;
          }
          Value val;
          val.set_null();
          one_table_one_record.push_back(val);
        }else {
          one_table_one_record.push_back((*values_)[0][offset]);
        }
      }
      // values_map[ori_table].push_back(one_table_one_record);
      Record record;
      RC rc = const_cast<Table *>(ori_table)->make_record(one_table_one_record.size(), one_table_one_record.data(), record);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to make record. rc=%s", strrc(rc));
        return rc;
      }
      rc = trx->insert_record(const_cast<Table *>(ori_table), record);
      if (rc != RC::SUCCESS) {
        LOG_ERROR("failed to insert record");
        return rc;
      }
    }
    return RC::SUCCESS;

  }

  RC rc = RC::SUCCESS;
  // 初始化所有的记录
  for (int i = 0; i < count; i++) {
    Record       record;
    const Value &value = values_->operator[](i)[0];
    rc                 = table_->make_record(values_->operator[](i).size(), values_->operator[](i).data(), record);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to make record. rc=%s", strrc(rc));
      return rc;
    }
    records.push_back(record);
  }
  // 插入所有的记录，success字段用来做错误回退
  int success;
  for (success = 0; success < count; ++success) {
    Table *table = records[success].table();
    if (table == nullptr) {
      table = table_;
    }
    rc = trx->insert_record(table, records[success]);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to insert record by transaction. rc=%s", strrc(rc));
      goto failed_to_insert;
    }
  }
  return rc;

failed_to_insert:
  // 错误回退
  // TODO 解决回退失败
  for (int i = 0; i < success; ++i) {
    rc = trx->delete_record(table_, records[i]);
    if (rc != RC::SUCCESS) {
      LOG_ERROR("Failed to rollback record by transaction. rc=%s, no=%d", strrc(rc), i);
    }
  }
  return rc;
}

RC InsertPhysicalOperator::next() { return RC::RECORD_EOF; }

RC InsertPhysicalOperator::close() { return RC::SUCCESS; }
