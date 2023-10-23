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
// Created by WangYunlai on 2022/12/30.
//

#include "sql/operator/join_physical_operator.h"

NestedLoopJoinPhysicalOperator::NestedLoopJoinPhysicalOperator() {}

RC NestedLoopJoinPhysicalOperator::open(Trx* trx) {
    if (children_.size() != 2) {
        LOG_WARN("nlj operator should have 2 children");
        return RC::INTERNAL;
    }

    RC rc = RC::SUCCESS;
    left_ = children_[0].get();
    right_ = children_[1].get();
    right_closed_ = true;
    round_done_ = true;

    rc = left_->open(trx);
    trx_ = trx;
    return rc;
}

RC NestedLoopJoinPhysicalOperator::next() {
    RC rc = RC::SUCCESS;
    // 如果是第一次获取,加载整个右表到内存
    if (is_first_) {
        is_first_ = false;
        rc = fetch_right_table();
        if (RC::SUCCESS != rc) {
            LOG_ERROR("Fetch Right Table Failed");
            return rc;
        }
        rht_it_ = filtered_rht_.end();
    }
    // 如果需要不遍历下一条左表的记录
    if (filtered_rht_.end() != rht_it_) {
        joined_tuple_.set_right_record(rht_[*rht_it_]);
        rht_it_++;
        return RC::SUCCESS;
    }
    // 如果需要遍历下一条左表的记录
    rc = left_next();
    if (RC::SUCCESS == rc) {
        // 重新过滤右表的数据
        rc = filter_right_table();
        if (RC::SUCCESS != rc) {
            LOG_ERROR("Filter Right Table Failed. RC = %d : %s", rc, strrc(rc));
            return rc;
        }
        return next();
    }

    if (RC::RECORD_EOF == rc) {
        LOG_INFO("Fetch Left Record EOF");
    } else {
        LOG_ERROR("Fetch Left Next Failed");
    }

    return rc;
}

RC NestedLoopJoinPhysicalOperator::close() {
    RC rc = left_->close();
    if (rc != RC::SUCCESS) {
        LOG_WARN("failed to close left oper. rc=%s", strrc(rc));
    }

    if (!right_closed_) {
        rc = right_->close();
        if (rc != RC::SUCCESS) {
            LOG_WARN("failed to close right oper. rc=%s", strrc(rc));
        } else {
            right_closed_ = true;
        }
    }
    return rc;
}

Tuple* NestedLoopJoinPhysicalOperator::current_tuple() {
    return &joined_tuple_;
}

RC NestedLoopJoinPhysicalOperator::left_next() {
    RC rc = RC::SUCCESS;
    rc = left_->next();
    if (rc != RC::SUCCESS) {
        return rc;
    }

    left_tuple_ = left_->current_tuple();
    joined_tuple_.set_left(left_tuple_);
    return rc;
}

RC NestedLoopJoinPhysicalOperator::right_next() {
    RC rc = RC::SUCCESS;
    if (round_done_) {
        if (!right_closed_) {
            rc = right_->close();
            right_closed_ = true;
            if (rc != RC::SUCCESS) {
                return rc;
            }
        }

        rc = right_->open(trx_);
        if (rc != RC::SUCCESS) {
            return rc;
        }
        right_closed_ = false;

        round_done_ = false;
    }

    rc = right_->next();
    if (rc != RC::SUCCESS) {
        if (rc == RC::RECORD_EOF) {
            round_done_ = true;
        }
        return rc;
    }

    right_tuple_ = right_->current_tuple();
    joined_tuple_.set_right(right_tuple_);
    return rc;
}

RC NestedLoopJoinPhysicalOperator::filter(JoinedTuple& tuple, bool& result) {
    RC rc = RC::SUCCESS;
    Value value;
    for (std::unique_ptr<Expression>& expr : predicates_) {
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

void NestedLoopJoinPhysicalOperator::set_predicates(std::vector<std::unique_ptr<Expression>>&& exprs) {
    predicates_ = std::move(exprs);
}

RC NestedLoopJoinPhysicalOperator::fetch_right_table() {
    RC rc = RC::SUCCESS;
    while (RC::SUCCESS == (rc = right_next())) {
        // store right records
        CompoundRecord cpd_rcd;
        right_tuple_->get_record(cpd_rcd);
        // need to deep copy the rcd and push back to rht
        // remember to delete them in dtor
        for (auto& rcd_ptr : cpd_rcd) {
            rcd_ptr = new Record(*rcd_ptr);
            rht_.emplace_back(*rcd_ptr);
        }
    }
    LOG_INFO("Fetch Right Table Success! There are %d rows in right table.", rht_.size());
    if (RC::RECORD_EOF == rc) {
        return RC::SUCCESS;
    }
    return rc;
}

RC NestedLoopJoinPhysicalOperator::filter_right_table() {
    RC rc = RC::SUCCESS;
    filtered_rht_.clear();
    rht_it_ = filtered_rht_.end();
    for (size_t i = 0; i < rht_.size(); ++i) {
        joined_tuple_.set_right_record(rht_[i]);
        bool filter_result = false;
        if (RC::SUCCESS != (rc = filter(joined_tuple_, filter_result))) {
            LOG_ERROR("JoinOperater do predicate Failed. RC = %d : %s", rc, strrc(rc));
            return rc;
        }
        if (filter_result) {
            filtered_rht_.emplace_back(i);
            LOG_INFO("add Row: %d",i);
        }
    }
    rht_it_ = filtered_rht_.begin();
    LOG_INFO("Filter Right Table Success! There are %d rows in right table satisfy predicates.", filtered_rht_.size());
    return rc;
}