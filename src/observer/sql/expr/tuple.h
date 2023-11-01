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
// Created by Wangyunlai on 2021/5/14.
//

#pragma once

#include <memory>
#include <vector>
#include <string>

#include "common/log/log.h"
#include "sql/expr/tuple_cell.h"
#include "sql/parser/parse.h"
#include "sql/parser/value.h"
#include "sql/expr/expression.h"
#include "storage/record/record.h"
#include "common/lang/bitmap.h"

class Table;

/**
 * @defgroup Tuple
 * @brief Tuple 元组，表示一行数据，当前返回客户端时使用
 * @details 
 * tuple是一种可以嵌套的数据结构。
 * 比如select t1.a+t2.b from t1, t2;
 * 需要使用下面的结构表示：
 * @code {.cpp}
 *  Project(t1.a+t2.b)
 *        |
 *      Joined
 *      /     \
 *   Row(t1) Row(t2)
 * @endcode
 * 
 */

/**
 * @brief 元组的结构，包含哪些字段(这里成为Cell)，每个字段的说明
 * @ingroup Tuple
 */
class TupleSchema 
{
public:
  void append_cell(const TupleCellSpec &cell)
  {
    cells_.push_back(cell);
  }
  void append_cell(const char *table, const char *field, AggFuncType aggrfunc_type = FUNC_NONE)
  {
    append_cell(TupleCellSpec(table, field, nullptr, aggrfunc_type));
  }
  void append_cell(const char *alias, AggFuncType aggrfunc_type = FUNC_NONE)
  {
    append_cell(TupleCellSpec(alias, aggrfunc_type));
  }
  int cell_num() const
  {
    return static_cast<int>(cells_.size());
  }
  const TupleCellSpec &cell_at(int i) const
  {
    return cells_[i];
  }

private:
  std::vector<TupleCellSpec> cells_;
};

/**
 * @brief 元组的抽象描述
 * @ingroup Tuple
 */
class Tuple 
{
public:
  Tuple() = default;
  virtual ~Tuple() = default;

  /**
   * @brief 获取元组中的Cell的个数
   * @details 个数应该与tuple_schema一致
   */
  virtual int cell_num() const = 0;

  /**
   * @brief 获取指定位置的Cell
   * 
   * @param index 位置
   * @param[out] cell  返回的Cell
   */
  virtual RC cell_at(int index, Value &cell) const = 0;

  /**
   * @brief 根据cell的描述，获取cell的值
   * 
   * @param spec cell的描述
   * @param[out] cell 返回的cell
   */
  virtual RC find_cell(const TupleCellSpec &spec, Value &cell) const = 0;

  virtual std::string to_string() const
  {
    std::string str;
    const int cell_num = this->cell_num();
    for (int i = 0; i < cell_num - 1; i++) {
      Value cell;
      cell_at(i, cell);
      str += cell.to_string();
      str += ", ";
    }

    if (cell_num > 0) {
      Value cell;
      cell_at(cell_num - 1, cell);
      str += cell.to_string();
    }
    return str;
  }

  virtual void set_record(std::vector<Record *> &records) {
    LOG_ERROR("Unimplenment, please check your tuple implenment set_record!");
  }

  virtual void set_right_record(std::vector<Record *> &records) {
    LOG_ERROR("Unimplenment, please check your tuple implenment set_record!");
  }

  virtual RC get_record(std::vector<Record *> &records) const {
    return RC::INTERNAL;
  }
};

/**
 * @brief 一行数据的元组
 * @ingroup Tuple
 * @details 直接就是获取表中的一条记录
 */
class RowTuple : public Tuple 
{
public:
  RowTuple() = default;

  virtual ~RowTuple()
  {
    for (FieldExpr *spec : speces_) {
      delete spec;
    }
    speces_.clear();
  }

  void set_record(std::vector<Record *> &records) override {
    assert(!records.empty());
    set_record(records.front());
  }

  void set_right_record(std::vector<Record *> &records) override {
    assert(!records.empty());
    set_record(records);
  }

  void set_record(Record *record)
  {
    this->record_ = record;
  }

  void set_schema(const Table *table, const std::vector<FieldMeta> *fields)
  {
    table_ = table;
    this->speces_.reserve(fields->size());
    for (const FieldMeta &field : *fields) {
      speces_.push_back(new FieldExpr(table, &field));
    }
  }

  int cell_num() const override
  {
    return speces_.size();
  }

  RC cell_at(int index, Value &cell) const override
  {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      LOG_WARN("invalid argument. index=%d", index);
      return RC::INVALID_ARGUMENT;
    }
    
    int bitmap_len = record_->bitmap_len();
    char *data = record_->data();
    common::Bitmap bitmap(data, bitmap_len);
    if (bitmap.get_bit(index)) {
      cell.set_type(NULLS);
      cell.set_data("NULL", 5);
    }else {
      FieldExpr *field_expr = speces_[index];
      const FieldMeta *field_meta = field_expr->field().meta();
      cell.set_type(field_meta->type());
      cell.set_data(data + field_meta->offset(), field_meta->len());
    }
    return RC::SUCCESS;
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override
  {
    const char *table_name = spec.table_name();
    const char *field_name = spec.field_name();
    if (!common::is_blank(table_name) && 0 != strcmp(table_name, table_->name())) {
      return RC::NOTFOUND;
    }

    for (size_t i = 0; i < speces_.size(); ++i) {
      const FieldExpr *field_expr = speces_[i];
      const Field &field = field_expr->field();
      if (0 == strcmp(field_name, field.field_name())) {
        return cell_at(i, cell);
      }
    }
    return RC::NOTFOUND;
  }

#if 0
  RC cell_spec_at(int index, const TupleCellSpec *&spec) const override
  {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      LOG_WARN("invalid argument. index=%d", index);
      return RC::INVALID_ARGUMENT;
    }
    spec = speces_[index];
    return RC::SUCCESS;
  }
#endif

  Record &record()
  {
    return *record_;
  }

  const Record &record() const
  {
    return *record_;
  }
  // 把get_record全部实现
  RC get_record(std::vector<Record *> &records) const override {
    records.push_back(new Record(*record_));
    return RC::SUCCESS;
  }

private:
  Record *record_ = nullptr;
  const Table *table_ = nullptr;
  std::vector<FieldExpr *> speces_;
};

/**
 * @brief 从一行数据中，选择部分字段组成的元组，也就是投影操作
 * @ingroup Tuple
 * @details 一般在select语句中使用。
 * 投影也可以是很复杂的操作，比如某些字段需要做类型转换、重命名、表达式运算、函数计算等。
 * 当前的实现是比较简单的，只是选择部分字段，不做任何其他操作。
 */
class ProjectTuple : public Tuple 
{
public:
  ProjectTuple() = default;
  virtual ~ProjectTuple()
  {
    for (TupleCellSpec *spec : speces_) {
      delete spec;
    }
    speces_.clear();
  }

  const Tuple *tuple() {
    return tuple_;
  }

  void set_tuple(Tuple *tuple)
  {
    this->tuple_ = tuple;
  }

  void add_cell_spec(TupleCellSpec *spec, AggFuncType type)
  {
    speces_.push_back(spec);
    types_.push_back(type);
  }
  
  int cell_num() const override
  {
    return speces_.size();
  }

const std::vector<AggFuncType>& func_types() {
  return types_;
}

RC cell_at(int index, Value &cell) const override
{
  if (index < 0 || index >= static_cast<int>(speces_.size())) {
    return RC::INTERNAL;
  }
  if (tuple_ == nullptr) {
    return RC::INTERNAL;
  }
  // TODO 当前实现算数表达式较为丑陋，可以尝试优化
  Expression *expr = speces_.operator[](index)->expression();
  if (expr != nullptr) {
    if (expr->type() == ExprType::ARITHMETIC) {
      return expr->get_value(*tuple_, cell);
    }
    if (expr->type() == ExprType::FUNC) {
      return expr->get_value(*tuple_, cell);
    }
  }
  // 解决project的查询的schema与其他算子schema不同的情况, 实现较为丑陋(对性能不友好)
  return tuple_->find_cell(*speces_[index], cell);
}

RC find_cell(const TupleCellSpec &spec, Value &cell) const override
{
  RC rc = tuple_->find_cell(spec, cell);
  return rc;
}

const std::vector<TupleCellSpec *>& speces() {
  return speces_;
}

RC get_record(std::vector<Record *> &record) const override {
  return tuple_->get_record(record);
} 

void set_record(std::vector<Record *> &record) override {
  tuple_->set_record(record);
}

void set_right_record(std::vector<Record *> &record) override {
  tuple_->set_right_record(record);
}

#if 0
  RC cell_spec_at(int index, const TupleCellSpec *&spec) const override
  {
    if (index < 0 || index >= static_cast<int>(speces_.size())) {
      return RC::NOTFOUND;
    }
    spec = speces_[index];
    return RC::SUCCESS;
  }
#endif
private:
  std::vector<TupleCellSpec *> speces_;
  std::vector<AggFuncType> types_;
  Tuple *tuple_ = nullptr;
};

class ExpressionTuple : public Tuple 
{
public:
  ExpressionTuple(std::vector<std::unique_ptr<Expression>> &expressions)
    : expressions_(expressions)
  {
  }
  
  virtual ~ExpressionTuple()
  {
  }

  int cell_num() const override
  {
    return expressions_.size();
  }

  RC cell_at(int index, Value &cell) const override
  {
    if (index < 0 || index >= static_cast<int>(expressions_.size())) {
      return RC::INTERNAL;
    }

    const Expression *expr = expressions_[index].get();
    return expr->try_get_value(cell);
  }

  RC find_cell(const TupleCellSpec &spec, Value &cell) const override
  {
    for (const std::unique_ptr<Expression> &expr : expressions_) {
      if (0 == strcmp(spec.alias(), expr->name().c_str())) {
        return expr->try_get_value(cell);
      }
    }
    return RC::NOTFOUND;
  }


private:
  const std::vector<std::unique_ptr<Expression>> &expressions_;
};

/**
 * @brief 一些常量值组成的Tuple
 * @ingroup Tuple
 */
class ValueListTuple : public Tuple 
{
public:
  ValueListTuple() = default;
  virtual ~ValueListTuple() = default;

  void set_cells(const std::vector<Value> &cells)
  {
    cells_ = cells;
  }

  virtual int cell_num() const override
  {
    return static_cast<int>(cells_.size());
  }

  virtual RC cell_at(int index, Value &cell) const override
  {
    if (index < 0 || index >= cell_num()) {
      return RC::NOTFOUND;
    }

    cell = cells_[index];
    return RC::SUCCESS;
  }

  virtual RC find_cell(const TupleCellSpec &spec, Value &cell) const override
  {
    return RC::INTERNAL;
  }

private:
  std::vector<Value> cells_;
};

/**
 * @brief 将两个tuple合并为一个tuple
 * @ingroup Tuple
 * @details 在join算子中使用
 */
class JoinedTuple : public Tuple 
{
public:
  JoinedTuple() = default;
  virtual ~JoinedTuple() = default;

  void set_left(Tuple *left)
  {
    left_ = left;
  }
  void set_right(Tuple *right)
  {
    right_ = right;
  }

  int cell_num() const override
  {
    return left_->cell_num() + right_->cell_num();
  }

  RC cell_at(int index, Value &value) const override
  {
    const int left_cell_num = left_->cell_num();
    if (index > 0 && index < left_cell_num) {
      return left_->cell_at(index, value);
    }

    if (index >= left_cell_num && index < left_cell_num + right_->cell_num()) {
      return right_->cell_at(index - left_cell_num, value);
    }

    return RC::NOTFOUND;
  }

  RC find_cell(const TupleCellSpec &spec, Value &value) const override
  {
    RC rc = left_->find_cell(spec, value);
    if (rc == RC::SUCCESS || rc != RC::NOTFOUND) {
      return rc;
    }

    return right_->find_cell(spec, value);
  }

  void set_record(std::vector<Record *> &record) {
    // 此处是断言JoinTuple的子Tuple必定是RowTuple
    // 这样做其实不好，但是遇到了一个bug: 如果这里没有断言的话
    // 就只能用left_->set_record(record);right_->set_record(record);插入record
    // 但是每次插入的时候又没有做首指针的移动, 会导致left_和right_持有同一个record(严重的bug)
    // 当产生这个bug的时候，会使上层算子计算时出现很多意想不到的问题
    // 如果是在这个方法移动指针的话又不好, 因为这里的std::vector<Record *> &record是别的类管理的
    // 如果移动之后，集合中的Record *没有被其他地方引用的话, 可能导致内存泄漏
    RowTuple *left = dynamic_cast<RowTuple *>(left_);
    RowTuple *right = dynamic_cast<RowTuple *>(right_);
    assert(record.size() >= 2);
    left->set_record(record[0]);
    right->set_record(record[1]);
  }

  void set_right_record(Record& record) {
    std::vector<Record*> tempRecord;
    tempRecord.emplace_back(&record);
    right_->set_right_record(tempRecord);
  }

  RC get_record(std::vector<Record *> &record) const override {
    left_->get_record(record);
    right_->get_record(record);
    return RC::SUCCESS;
  } 
  
private:
  Tuple *left_ = nullptr;
  Tuple *right_ = nullptr;
};

/**
 * @brief 分组的tuple
 * @details 在group算子中使用 
 */
class GroupTuple : public Tuple {
public:
  GroupTuple() = default;
  
  GroupTuple(const std::vector<FieldExpr *> &field_exprs, const std::vector<AggrFuncExpr *> &aggrfunc_exprs) {
    aggr_results_.resize(aggrfunc_exprs.size());
    field_results_.resize(field_exprs.size());
    all_null_.resize(aggrfunc_exprs.size());
    counts_.resize(aggrfunc_exprs.size());

    field_exprs_ = field_exprs;
    aggr_func_exprs_ = aggrfunc_exprs;
  }

  virtual ~GroupTuple() {
    // TODO manage memory
    // 正常来说tuple_肯定不用清理, 需要关注一下field_exprs_和aggr_func_exprs_. 但我感觉应该也是不用清理的
  }

  void set_tuple(Tuple *tuple) {
    this->tuple_ = tuple;
  }

  int cell_num() const override {
    return tuple_->cell_num();
  }

  virtual RC cell_at(int index, Value &cell) const override {
    // 目前这样应该足够使用? 有待DEBUG
    return RC::UNIMPLENMENT;
  }

  virtual RC find_cell(const TupleCellSpec &spec, Value &cell) const override {
    if (spec.aggfunc_type() != FUNC_NONE) {
      for (int i = 0; i < aggr_func_exprs_.size(); i++) {
        AggrFuncExpr *expr = aggr_func_exprs_[i];
        if (expr->aggr_type() != spec.aggfunc_type()) {
          continue;
        }
        const ValueExpr *value_expr = expr->value();
        if (value_expr) {
          cell = aggr_results_[i];
          return RC::SUCCESS;
        }

        if ((expr->field() != nullptr && strcmp(expr->field()->field_name(), spec.field_name()) == 0) || 
            (expr->value() != nullptr && strcmp(expr->value()->get_value().get_string().c_str(), spec.field_name()) == 0)) {
              cell = aggr_results_[i];
              return RC::SUCCESS;
        }
      }
      LOG_WARN("Couldn't find %s", spec.to_string().c_str());
      return RC::NOTFOUND;
    }

    for (int i = 0; i < field_exprs_.size(); i++) {
      FieldExpr *expr = field_exprs_[i];
      if (common::is_blank(spec.table_name())) {
        if (strcmp(expr->field_name(), spec.field_name()) != 0) {
          continue;
        }
        cell = field_results_[i];
        return RC::SUCCESS;
      }
      if (strcmp(expr->table_name(), spec.table_name()) == 0 
          && strcmp(expr->field_name(), spec.field_name()) == 0) {
          cell = field_results_[i];
          return RC::SUCCESS;
      }
    }
    LOG_WARN("Couldn't find %s", spec.to_string().c_str());
    return RC::NOTFOUND;
  }

  void do_aggregate_when_first_eof() {
    assert(field_exprs_.size() < 1 && aggr_func_exprs_.size() > 0);
    for (int i = 0; i < counts_.size(); ++i) {
      counts_[i] = 0;
      all_null_[i] = true;
    }
    for (int i = 0; i < aggr_results_.size(); ++i) {
      switch(aggr_func_exprs_[i]->aggr_type()) {
        case AggFuncType::FUNC_COUNT: {
          aggr_results_[i].set_int(0);
        }break;
        
        default: {
          aggr_results_[i].set_null();
        }break;
      }
    }
  }
  /**
   * 当某个分组第一次聚合时，则调用该函数
   */
  void do_aggregate_first();
  /**
   * 当某个分组非第一次聚合时，调用该函数
   */
  void do_aggregate();
  /**
   * 数据读取完毕后，对所有分组调用该方法
   */
  void do_aggregate_done();

  const std::vector<FieldExpr *>& field_exprs() const {
    return field_exprs_;
  }

  const std::vector<AggrFuncExpr *>& aggr_func_exprs() const {
    return aggr_func_exprs_;
  }

private:
  std::vector<bool> all_null_;        ///< 用于记录某个分组中的某一列记录是否全部为null
  std::vector<int> counts_;           ///< 用于记录某个分组中除了null之外，有多少条数据
  std::vector<Value> aggr_results_;   ///< 用于存储聚合函数的结果
  std::vector<Value> field_results_;  ///< 用于存储非聚合函数的结果

  std::vector<FieldExpr *> field_exprs_;
  std::vector<AggrFuncExpr *> aggr_func_exprs_;
  Tuple *tuple_ = nullptr;
};
