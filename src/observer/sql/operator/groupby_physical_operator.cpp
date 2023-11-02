#include "sql/operator/groupby_physical_operator.h"
#include "common/rc.h"
#include "storage/trx/trx.h"

GroupByPhysicalOperator::GroupByPhysicalOperator(const std::vector<GroupByUnit *> *units, 
    const std::vector<AggrFuncExpr *> &aggr_exprs,
    const std::vector<FieldExpr *> &field_exprs) {
  units_ = units;
  tuple_ = GroupTuple(field_exprs, aggr_exprs);
}

RC GroupByPhysicalOperator::open(Trx *trx) {
  RC rc = RC::SUCCESS;
  if ((rc = children_[0]->open(trx)) != RC::SUCCESS) {
    return rc;
  }
  is_record_eof_ = false;
  is_first_ = true;
  is_new_group_ = true;
  tuple_.set_tuple(children_[0]->current_tuple());
  return rc;
}

RC GroupByPhysicalOperator::next() {
  if (is_record_eof_) {
    return RC::RECORD_EOF;
  }
  // 初次进入，就要对上一条记录的组记录进行初始化
  if (is_first_) {
    RC rc = children_[0]->next();
    if (RC::SUCCESS != rc) {
      // 说明是一张空表, 空表可以返回COUNT和SUM值
      if (rc == RC::RECORD_EOF) {
        int field_expr_size = tuple_.field_exprs().size();
        int aggr_expr_size = tuple_.aggr_func_exprs().size();
        if (field_expr_size == 0 && aggr_expr_size != 0) {
          tuple_.do_aggregate_when_first_eof();
          is_record_eof_ = true;
          return RC::SUCCESS;
        }
      }
      return rc;
    }
    is_first_ = false;
    is_new_group_ = true;
    // set initial value of pre_values_
    for (auto *unit : *units_) {
      pre_group_val_.emplace_back(Value());
      unit->expr()->get_value(*children_[0]->current_tuple(), pre_group_val_.back());
    }
    LOG_INFO("GroupByOperator set first success!");
  }
  
  RC rc = RC::SUCCESS;
  while (true) {
    if (is_new_group_) {
      tuple_.do_aggregate_first();
      is_new_group_ = false;
    }
    
    if (RC::SUCCESS != (rc = children_[0]->next())) {
      break;
    }
    // 判断该记录是否为新的组, 若不是，则更新上一条记录的组的标识数据
    for (int i = 0; i < units_->size(); i++) {
      Value tmp;
      (*units_)[i]->expr()->get_value(*(children_[0]->current_tuple()), tmp);
      // 分组列的值是NULL的话，属于同一个组
      if (pre_group_val_[i].attr_type() == NULLS && tmp.attr_type() == NULLS) {
        continue;
      }
      if (pre_group_val_[i] != tmp) {
        is_new_group_ = true;
        pre_group_val_[i] = tmp;
      }
    }
    if (is_new_group_) {
      tuple_.do_aggregate_done();
      return rc;
    }
    tuple_.do_aggregate();
  }
  if (rc == RC::RECORD_EOF) {
    is_record_eof_ = true;
    tuple_.do_aggregate_done();
    return RC::SUCCESS;
  }
  return rc;
}

RC GroupByPhysicalOperator::close() {
  return children_[0]->close();
}

Tuple *GroupByPhysicalOperator::current_tuple() {
  return &tuple_;
}