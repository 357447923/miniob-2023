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
// Created by Wangyunlai.wyl on 2021/5/18.
//

#include "storage/index/index_meta.h"
#include "storage/field/field_meta.h"
#include "storage/table/table_meta.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "json/json.h"
#include "storage/index/index_type.h"

const static Json::StaticString FIELD_NAME("name");
const static Json::StaticString INDEX_TYPE("index_type");
const static Json::StaticString FIELD_FIELD_NAME("field_name");

RC IndexMeta::init(const char *name, IndexType indexType, std::vector<std::string> fields)
{
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init index, name is empty.");
    return RC::INVALID_ARGUMENT;
  }
  name_ = name;
  fields_ = fields;
  indexType_ = indexType;
  return RC::SUCCESS;
}

void IndexMeta::to_json(Json::Value &json_value) const
{
  json_value[FIELD_NAME] = name_;
  json_value[INDEX_TYPE] = indexToString(indexType_);
  for (size_t i = 0; i < fields_.size(); i++) {
    int idx = i;
    json_value[FIELD_FIELD_NAME][idx] = fields_[idx];
  }
}

RC IndexMeta::from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index)
{
  const Json::Value &name_value = json_value[FIELD_NAME];
  const Json::Value &index_type_value = json_value[INDEX_TYPE];
  const Json::Value &field_value = json_value[FIELD_FIELD_NAME];
  if (!name_value.isString()) {
    LOG_ERROR("Index name is not a string. json value=%s", name_value.toStyledString().c_str());
    return RC::INDEX_GENERIC_ERROR;
  }

  if (index_type_value.isString()) {
    IndexType indexType =  stringToIndex(index_type_value.asCString());
    if (indexType < IndexType::NORMAL_IDX||indexType > IndexType::UNIQUE_IDX)
    {
      LOG_ERROR("Index is unknown. json value=%s", index_type_value.asCString());
    }
    return RC::INDEX_GENERIC_ERROR;
  }

  std::vector<std::string> fields;
  for (size_t i = 0; i < field_value.size(); i++) {
    int idx = i;
    if (!field_value[idx].isString()) {
      LOG_ERROR("Field name of index [%s] is not a string. json value=%s",
          name_value.asCString(),
          field_value.toStyledString().c_str());
      return RC::INDEX_GENERIC_ERROR;
    }

    const FieldMeta *field = table.field(field_value[idx].asCString());
    if (nullptr == field) {
      LOG_ERROR("Deserialize index [%s]: no such field: %s", name_value.asCString(), field_value.asCString());
      return RC::SCHEMA_FIELD_MISSING;
    }
    fields.push_back(field->name());
  }

  return index.init(name_value.asCString(), IndexType::NORMAL_IDX, fields);
}

const char *IndexMeta::name() const
{
  return name_.c_str();
}

const std::vector<std::string> *IndexMeta::fields() const
{
  return &fields_;
}

const IndexType IndexMeta::indexType() const
{
  return indexType_;
}

void IndexMeta::desc(std::ostream &os) const
{
  os << "index name=" << name_ << ", field=" << fields_[0];

  for (size_t i = 1; i < fields_.size(); i++) {
    os << ',' << fields_[i];
  }
}