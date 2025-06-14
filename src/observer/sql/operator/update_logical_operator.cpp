#include "sql/operator/update_logical_operator.h"

UpdateLogicalOperator::UpdateLogicalOperator(Table *table, std::unordered_map<std::string, Expression *> update_map) {
    table_ = table;
    update_map_ = std::move(update_map);
}

