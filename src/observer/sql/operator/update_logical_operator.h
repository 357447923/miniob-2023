#ifndef MINIOB_SQL_OPERATOR_UPDATE_LOGICAL_OPERATOR_H
#define MINIOB_SQL_OPERATOR_UPDATE_LOGICAL_OPERATOR_H

#include "sql/operator/logical_operator.h"
#include "sql/parser/parse_defs.h"
#include <unordered_map>

/**
 * @brief 逻辑算子，用于执行update语句
 * @ingroup LogicalOperator
 */
class UpdateLogicalOperator : public LogicalOperator {

public:
    UpdateLogicalOperator(Table *table,std::unordered_map<std::string, Expression*> update_map);

    ~UpdateLogicalOperator() override = default;

    LogicalOperatorType type() const override {
        return LogicalOperatorType::UPDATE;
    }

    inline Table * table() const {
        return table_;
    }

    const std::unordered_map<std::string, Expression *> update_map() const {
        return update_map_;
    }

private:
    Table * table_ = nullptr;
    std::unordered_map<std::string, Expression *> update_map_ ;
};

#endif