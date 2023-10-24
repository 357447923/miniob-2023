#include "sql/expr/tuple.h"

void GroupTuple::do_aggregate_first() {
  // 先假设所有的聚合字段全部是null
  for (int i = 0; i < all_null_.size(); i++) {
    all_null_[i] = true;
    counts_[i] = 0;
  }
  // 先从本分组的第一条数据中取出数据，存放到聚合函数结果中
  for (int i = 0; i < aggr_func_exprs_.size(); i++) {
    const AggrFuncExpr *aggrfunc_expr = aggr_func_exprs_[i];
    aggrfunc_expr->get_value(*tuple_, aggr_results_[i]);
    // 不为null的话，必须进行记录
    if (!aggr_results_[i].is_null()) {
      all_null_[i] = false;
      counts_[i]++;
    }
  }
  for (int i = 0; i < field_exprs_.size(); i++) {
    const FieldExpr *field_expr = field_exprs_[i];
    field_expr->get_value(*tuple_, field_results_[i]);
  }
}

void GroupTuple::do_aggregate() {
  Value tmp;
  for (int i = 0; i < aggr_func_exprs_.size(); i++) {
    aggr_func_exprs_[i]->get_value(*tuple_, tmp);
    if (tmp.is_null()) {
      // null无法聚合
      continue;
    }
    all_null_[i] = false;
    counts_[i]++;
    AggFuncType type = aggr_func_exprs_[i]->aggr_type();
    if (type == AggFuncType::FUNC_COUNT) {
      continue;
    }
    // 处理可能为null的结果
    if (aggr_results_[i].is_null()) {
      aggr_results_[i] = tmp;
      continue;
    }
    // 根据不同聚合函数类型，做出不同操作
    switch (type)
    {
      // MAX和MIN都是直接替换
    case AggFuncType::FUNC_MAX: {
      RC rc = aggr_results_[i].compare(tmp);
      if (rc == RC::LEFT_LT_ANOTHER) {
        aggr_results_[i].set_data(tmp.data(), tmp.length());
      }
      break;
    }
    case AggFuncType::FUNC_MIN: {
      if (aggr_results_[i].compare(tmp) == RC::LEFT_GT_ANOTHER) {
        aggr_results_[i].set_data(tmp.data(), tmp.length());
      }
      break;
    }
    // SUM和AVG都是先叠加
    case AggFuncType::FUNC_SUM: 
    case AggFuncType::FUNC_AVG: {
      Value &val = aggr_results_[i];
      if (val.attr_type() == INTS) {
        val.set_int(val.get_int() + tmp.get_int());
      }else {
        val.set_float(tmp.get_float() + val.get_float());
      }
      break;
    }
    default: {
      LOG_ERROR("Unsupported AggrFuncType");
      break;
    }
    }
  }
}

void GroupTuple::do_aggregate_done() {
  for (int i = 0; i < aggr_func_exprs_.size(); i++) {
    AggFuncType type = aggr_func_exprs_[i]->aggr_type();
    switch (type)
    {
    case AggFuncType::FUNC_AVG: {
      // 全是null值，则不需要处理
      if (all_null_[i]) {
        break;
      }
      Value &val = aggr_results_[i];
      if (val.attr_type() == INTS) {
        val.set_float(val.get_int() / (counts_[i] * 1.0));
      } else {
        val.set_float(val.get_float() / counts_[i]);
      }
      break;
    }
    case AggFuncType::FUNC_COUNT: {
      aggr_results_[i] = Value(counts_[i]);
      break;
    }
    default:
      break;
    }
  }
}