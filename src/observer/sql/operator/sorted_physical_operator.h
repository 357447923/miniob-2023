#pragma once

#include "sql/operator/physical_operator.h"
#include "sql/expr/tuple.h"
#include "sql/stmt/orderby_stmt.h"

class SortedPhysicalOperator : public PhysicalOperator {
public:
  SortedPhysicalOperator(const std::vector<OrderByUnit *> *units) : units_(units) {}
  virtual ~SortedPhysicalOperator();

  PhysicalOperatorType type() const override {
    return PhysicalOperatorType::SORTED;
  }

  RC open(Trx *trx) override;

  RC next() override;

  RC close() override;

  Tuple *current_tuple() override;

private:
/**
 * 收集表中的所有Record并且进行排序
 */
  RC fetch_and_sort_table();

private:
  // 标识是否是第一次使用该排序算子的next方法, 第一次必须先聚合, 等后期能用索引的时候，可能就要修改
  bool is_first = true;
  const std::vector<OrderByUnit *> *units_;
  std::vector<std::vector<Record *>> records_;
  // 标识排序后的顺序
  std::vector<int> ordered_idx_;
  // 用于标识第一条未读记录的位置
  std::vector<int>::iterator it_;
};