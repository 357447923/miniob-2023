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
// Created by Wangyunlai on 2022/6/6.
//

#include "sql/stmt/select_stmt.h"
#include <regex>
#include "common/lang/string.h"
#include "common/log/log.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/group_by_stmt.h"
#include "sql/stmt/orderby_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

SelectStmt::~SelectStmt() {
    if (nullptr != filter_stmt_) {
        delete filter_stmt_;
        filter_stmt_ = nullptr;
    }
    if (nullptr != groupby_stmt_) {
        delete groupby_stmt_;
        groupby_stmt_ = nullptr;
    }
    if (nullptr != orderby_for_groupby_) {
        delete orderby_for_groupby_;
        orderby_for_groupby_ = nullptr;
    }
    if (nullptr != orderby_stmt_) {
        delete orderby_stmt_;
        orderby_stmt_ = nullptr;
    }
    if (nullptr != having_stmt_) {
        delete having_stmt_;
        having_stmt_ = nullptr;
    }
    for (auto expr : query_expressions_) {
        delete expr;
    }
    query_expressions_.clear();
}

static void wildcard_fields(Table* table, std::vector<Expression*>& field_metas) {
    const TableMeta& table_meta = table->table_meta();
    const int field_num = table_meta.field_num();
    for (int i = table_meta.sys_field_num(); i < field_num; i++) {
        field_metas.push_back(new FieldExpr(table, table_meta.field(i)));
    }
}

static void clean_expr_when_fail(std::vector<Expression*>& exprs) {
    for (auto expr : exprs) {
        delete expr;
    }
    exprs.clear();
}

static bool handle_expr_when_mutiple_table(Expression *expr, const std::unordered_map<std::string, Table *> &table_map, 
        std::vector<Expression *> &query_expressions) {
    switch (expr->type()) {
        case ExprType::ARITHMETIC: {
            ArithmeticExpr* arithmetic_expr = (ArithmeticExpr*)expr;
            ArithmeticExpr::find_field_need(table_map, arithmetic_expr);
            query_expressions.push_back(expr);
            return true;
        }

        case ExprType::FUNC: {
            FuncExpr *func_expr = (FuncExpr *)expr;
            RC rc = FuncExpr::find_field_need(table_map, func_expr);
            if (rc != RC::SUCCESS) {
                return false;
            }
            query_expressions.push_back(expr);
            return true;
        }

        default: {
            LOG_ERROR("this expression can't handle by this handler");
            return false;
        }
    }
}

RC SelectStmt::create(Db* db, const SelectSqlNode& select_sql, const std::vector<Table *> &parent_tables,
        const std::unordered_map<std::string, Table *> &parent_table_map, Stmt*& stmt) {
    if (nullptr == db) {
        LOG_WARN("invalid argument. db is null");
        return RC::INVALID_ARGUMENT;
    }
    if (select_sql.relations.empty()) {
        if (select_sql.attributes.empty()) {
            return RC::SQL_SYNTAX;
        }
        std::vector<Expression*> query_expressions;
        for (auto &attr : select_sql.attributes) {
            if (attr.expression == nullptr || attr.expression->type() != ExprType::FUNC) {
                return RC::SQL_SYNTAX;
            }
            query_expressions.push_back(attr.expression);
        }
        SelectStmt *select_stmt = new SelectStmt();
        select_stmt->query_expressions_.swap(query_expressions);
        stmt = select_stmt;
        return RC::SUCCESS;
    }
    // collect tables in `from` statement
    std::vector<Table*> tables;
    std::unordered_map<std::string, Table*> table_map;
    std::unordered_set<std::string> alias_set;
    // std::unordered_map<Table *, std::string> alias_map;
    // 建立表名和表的映射关系
    for (size_t i = 0; i < select_sql.relations.size(); i++) {
        const char* table_name = select_sql.relations[i].table.c_str();
        const char* alias_name = select_sql.relations[i].alias.get();
        if (common::is_blank(table_name)) {
            LOG_WARN("invalid argument. relation name is null. index=%d", i);
            return RC::INVALID_ARGUMENT;
        }

        Table* table = db->find_table(table_name);
        if (nullptr == table) {
            LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
            return RC::SCHEMA_TABLE_NOT_EXIST;
        }

        if (nullptr != alias_name) {
            if (common::is_blank(alias_name) || table_map.contains(alias_name)) {
                return RC::SQL_SYNTAX;
            }
        }

        tables.push_back(table);
        table_map.insert(std::pair<std::string, Table*>(table_name, table));
        alias_set.insert(table_name);

        if (nullptr != alias_name) {
            table_map.insert(std::pair<std::string, Table *>(alias_name, table));
            alias_set.insert(alias_name);
            // alias_map.insert(std::pair<Table *, std::string>(table, alias_name));
        }
    }

    // collect query fields in `select` statement
    std::vector<Expression*> query_expressions;
    bool contains_aggr_func = false;
    for (int i = static_cast<int>(select_sql.attributes.size()) - 1; i >= 0; i--) {
        const RelAttrSqlNode& relation_attr = select_sql.attributes[i];
        if (common::is_blank(relation_attr.relation_name.c_str()) &&
            0 == strcmp(relation_attr.attribute_name.c_str(), "*")) {
            if (relation_attr.type == AggFuncType::FUNC_NONE) {
                for (Table* table : tables) {
                    wildcard_fields(table, query_expressions);
                }
                continue;
            }
            // '*'仅仅支持COUNT(*)
            if (relation_attr.type != FUNC_COUNT) {
                return RC::SQL_SYNTAX;
            }
            contains_aggr_func = true;
            query_expressions.push_back(new AggrFuncExpr(relation_attr.type, new ValueExpr(Value("*", 2))));
        } else if (!common::is_blank(relation_attr.relation_name.c_str())) {
            // 如果是 t.*就把'*'展开
            const char* table_name;
            const char* field_name;
            const char* alias = relation_attr.alias.get();
            if (alias) {
                if (alias_set.contains(alias)) {
                    return RC::SQL_SYNTAX;
                }
                alias_set.insert(alias);
            }

            if (relation_attr.expression == nullptr) {
                table_name = relation_attr.relation_name.c_str();
                field_name = relation_attr.attribute_name.c_str();
            } else {
                switch(relation_attr.expression->type()) {
                    case ExprType::ARITHMETIC: {
                        ArithmeticExpr* arithmetic_expr = (ArithmeticExpr*)relation_attr.expression;
                        ArithmeticExpr::find_field_need(table_map, arithmetic_expr);
                        if (arithmetic_expr->contains_aggr()) {
                            contains_aggr_func = true;
                        }
                        query_expressions.emplace_back(relation_attr.expression);
                    }break;
                    case ExprType::FUNC: {
                        FuncExpr *func_expr = (FuncExpr *)relation_attr.expression;
                        RC rc = FuncExpr::find_field_need(table_map, func_expr);
                        if (rc != RC::SUCCESS) {
                            return rc;
                        }
                        query_expressions.emplace_back(relation_attr.expression);
                    }break;
                    default: {
                        LOG_ERROR("file: %s, line: %d undefine", __FILE__, __LINE__);
                    }break;
                }
                continue;
            }

            if (0 == strcmp(table_name, "*")) {
                if (0 != strcmp(field_name, "*")) {
                    LOG_WARN("invalid field name while table is *. attr=%s", field_name);
                    clean_expr_when_fail(query_expressions);
                    return RC::SCHEMA_FIELD_MISSING;
                }
                for (Table* table : tables) {
                    wildcard_fields(table, query_expressions);
                }
            } else {
                Table *table;
                auto iter = table_map.find(table_name);
                if (iter != table_map.end()) {
                    table = iter->second;
                }else {
                    auto parent_iter = parent_table_map.find(table_name);
                    if (parent_iter == parent_table_map.end()) {
                        LOG_WARN("no such table in from list: %s", table_name);
                        clean_expr_when_fail(query_expressions);
                        return RC::SCHEMA_FIELD_MISSING;
                    }
                    table = parent_iter->second;
                }
                if (0 == strcmp(field_name, "*")) {
                    wildcard_fields(table, query_expressions);
                } else {
                    const FieldMeta* field_meta = table->table_meta().field(field_name);
                    if (nullptr == field_meta) {
                        LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), field_name);
                        clean_expr_when_fail(query_expressions);
                        return RC::SCHEMA_FIELD_MISSING;
                    }
                    if (relation_attr.type == FUNC_NONE) {
                        auto field_expr = new FieldExpr(table, field_meta);
                        field_expr->set_alias(alias);
                        query_expressions.emplace_back(field_expr);
                    } else {
                        contains_aggr_func = true;
                        auto aggr_expr = new AggrFuncExpr(relation_attr.type, new FieldExpr(table, field_meta));
                        aggr_expr->set_alias(alias);
                        query_expressions.emplace_back(aggr_expr);
                    }
                }
            }
        } else {
            const char *alias = relation_attr.alias.get();
            if (alias) {
                if (alias_set.contains(alias)) {
                    return RC::SQL_SYNTAX;
                }
                alias_set.insert(alias);
            }
            // select id from xxx(查询时字段没写表名的情况，miniob中不考虑select id from x1, x2中id只有一个表中存在的情况)
            if (tables.size() != 1) {
                if (!relation_attr.expression) {
                    LOG_ERROR("select schema should follow 'table.field' in miniob");
                    return RC::SQL_SYNTAX;
                }
                // 没写表名的时候也可能是select 1+1 这种情况
                if (!handle_expr_when_mutiple_table(relation_attr.expression, table_map, query_expressions)) {
                    LOG_WARN("invalid. I do not know the attr's table. attr=%s", relation_attr.attribute_name.c_str());
                    clean_expr_when_fail(query_expressions);
                    return RC::SCHEMA_FIELD_MISSING;
                }
                relation_attr.expression->set_alias(alias);
                if (relation_attr.expression->type() == ExprType::ARITHMETIC 
                        && static_cast<ArithmeticExpr *>(relation_attr.expression)->contains_aggr()) {
                    contains_aggr_func = true;
                }
                continue;
            }

            Table* table = tables[0];
            const FieldMeta* field_meta = nullptr;

            if (relation_attr.expression != nullptr) {
                switch (relation_attr.expression->type()) {
                    case ExprType::ARITHMETIC: {
                        // 填充算术表达式中的字段
                        ArithmeticExpr* arithmetic_expr = (ArithmeticExpr*)relation_attr.expression;
                        ArithmeticExpr::find_field_need(table, arithmetic_expr);
                        arithmetic_expr->set_alias(alias);
                        if (arithmetic_expr->contains_aggr()) {
                            contains_aggr_func = true;
                        }
                        query_expressions.emplace_back(arithmetic_expr);
                    }break;
                    case ExprType::FUNC: {
                        FuncExpr *func_expr = (FuncExpr *)relation_attr.expression;
                        RC rc = FuncExpr::find_field_need(table, func_expr);
                        if (rc != RC::SUCCESS) {
                            return rc;
                        }
                        func_expr->set_alias(alias);
                        query_expressions.emplace_back(func_expr);
                    }break;

                    default: {
                        LOG_ERROR("cannot handle this expression, type: %d", relation_attr.expression->type());
                        return RC::UNIMPLENMENT;
                    }
                }
                continue;
            }
            if (relation_attr.type == FUNC_NONE) {
                field_meta = table->table_meta().field(relation_attr.attribute_name.c_str());
                if (field_meta == nullptr) {
                    LOG_ERROR("Can't find field:'%s'", relation_attr.attribute_name.c_str());
                    clean_expr_when_fail(query_expressions);
                    return RC::SCHEMA_FIELD_NOT_EXIST;
                }
                auto field_expr = new FieldExpr(table, field_meta, FUNC_NONE);
                field_expr->set_alias(alias);
                query_expressions.push_back(field_expr);
            } else {
                const char* field_name = relation_attr.attribute_name.c_str();
                // 检查field_name是否符合: 仅有一个"*"或为纯数字
                if (std::regex_match(field_name, std::regex("^(\\*|\\d+)$"))) {
                    // 聚合函数中只有COUNT(*)，其他不能用*
                    if (relation_attr.type != FUNC_COUNT) {
                        LOG_INFO("func: %s can't receive '*' as param", AGGR_FUNC_TYPE_NAME[relation_attr.type]);
                        clean_expr_when_fail(query_expressions);
                        return RC::SQL_SYNTAX;
                    }
                    contains_aggr_func = true;
                    auto aggr_expr = new AggrFuncExpr(relation_attr.type, new ValueExpr(Value(field_name)));
                    aggr_expr->set_alias(alias);
                    query_expressions.push_back(aggr_expr);
                    continue;
                }
                field_meta = table->table_meta().field(field_name);
                if (field_meta == nullptr) {
                    LOG_ERROR("Can't find field:'%s'", field_name);
                    clean_expr_when_fail(query_expressions);
                    return RC::SCHEMA_FIELD_NOT_EXIST;
                }
                // 字符串类型不能使用SUM,AVG聚合函数
                if (field_meta->type() == AttrType::CHARS && (relation_attr.type == FUNC_SUM || relation_attr.type == FUNC_AVG)) {
                    LOG_INFO("func: %s can't receive type 'CHARS' as param", AGGR_FUNC_TYPE_NAME[relation_attr.type]);
                    clean_expr_when_fail(query_expressions);
                    return RC::SQL_SYNTAX;
                }
                contains_aggr_func = true;
                Expression* aggr_expr = new AggrFuncExpr(relation_attr.type, new FieldExpr(table, field_meta, relation_attr.type));
                aggr_expr->set_alias(alias);
                query_expressions.push_back(aggr_expr);
            }
        }
    }

    Table* default_table = nullptr;
    if (tables.size() == 1) {
        default_table = tables[0];
    }
    FilterStmt* inner_join_filter_stmt = nullptr;
    std::vector<FilterStmt*> inner_join_filter_stmts;
    FilterStmt* filter_stmt = nullptr;
    OrderByStmt* orderby_stmt = nullptr;
    GroupByStmt* groupby_stmt = nullptr;
    OrderByStmt* orderby_for_groupby = nullptr;
    HavingStmt* having_stmt = nullptr;
    SelectStmt* select_stmt = nullptr;

    std::unordered_map<std::string, Table *> temp_table_map = table_map;
    temp_table_map.insert(parent_table_map.begin(), parent_table_map.end());
    
    // create filter statement in `where` statement
    RC rc = FilterStmt::create(db,
                               default_table,
                               &temp_table_map,
                               tables,
                               select_sql.conditions.data(),
                               static_cast<int>(select_sql.conditions.size()),
                               filter_stmt);
    if (rc != RC::SUCCESS) {
        LOG_WARN("cannot construct filter stmt");
        goto err_handler;
    }
    // 构造inner_join_filter_stmt
    if (select_sql.join_conditions.size() != 0) {
        for (std::vector<ConditionSqlNode> join_condition : select_sql.join_conditions) {
            rc = FilterStmt::create(db, default_table, &temp_table_map, tables, join_condition.data(), static_cast<int>(join_condition.size()), inner_join_filter_stmt);
            if (rc != RC::SUCCESS) {
                LOG_WARN("cannot construct inner join filter stmt");
                goto err_handler;
            } else {
                inner_join_filter_stmts.push_back(inner_join_filter_stmt);
            }
        }
    }

    // 构造OrderStmt
    rc = OrderByStmt::create(db,
                             default_table,
                             &table_map,
                             select_sql.orders.data(),
                             static_cast<int>(select_sql.orders.size()),
                             orderby_stmt);
    if (rc != RC::SUCCESS) {
        LOG_WARN("cannot construct orderby stmt");
        goto err_handler;
    }
    if (contains_aggr_func) {
        // 构造GroupByStmt
        rc = GroupByStmt::create(db,
                                 default_table,
                                 &table_map,
                                 select_sql.groups.data(),
                                 static_cast<int>(select_sql.groups.size()),
                                 groupby_stmt);
        if (rc != RC::SUCCESS) {
            LOG_WARN("cannot construct groupby stmt");
            goto err_handler;
        }
    }
    if (!select_sql.groups.empty()) {
        if (!groupby_stmt) {
            rc = GroupByStmt::create(db,
                                     default_table,
                                     &table_map,
                                     select_sql.groups.data(),
                                     static_cast<int>(select_sql.groups.size()),
                                     groupby_stmt);
            if (rc != RC::SUCCESS) {
                LOG_WARN("cannot construct groupby stmt");
                goto err_handler;
            }
        }
        // 构造order_stmt_for_groupby
        rc = OrderByStmt::create(db,
                                 default_table,
                                 &table_map,
                                 select_sql.groups.data(),
                                 static_cast<int>(select_sql.groups.size()),
                                 orderby_for_groupby);
        if (rc != RC::SUCCESS) {
            LOG_WARN("cannot construct orderby stmt");
            goto err_handler;
        }
    }
    // having_stmt构造
    if (!select_sql.havings.empty()) {
        rc = HavingStmt::create(db, default_table, &table_map, tables, select_sql.havings.data(), select_sql.groups.size(), having_stmt);
        if (rc != RC::SUCCESS) {
            LOG_INFO("cannot construct having filter stmt");
            return rc;
        }
    }

    // 所有前置工作完成，开始构造SelectStmt
    select_stmt = new SelectStmt();
    select_stmt->tables_.swap(tables);
    select_stmt->query_expressions_.swap(query_expressions);
    select_stmt->inner_join_filter_stmts_.swap(inner_join_filter_stmts);
    select_stmt->filter_stmt_ = filter_stmt;
    select_stmt->groupby_stmt_ = groupby_stmt;
    select_stmt->orderby_for_groupby_ = orderby_for_groupby;
    select_stmt->orderby_stmt_ = orderby_stmt;
    select_stmt->having_stmt_ = having_stmt;
    stmt = select_stmt;
    return RC::SUCCESS;

err_handler:
    clean_expr_when_fail(query_expressions);
    if (filter_stmt != nullptr) {
        delete filter_stmt;
    }
    if (groupby_stmt != nullptr) {
        delete groupby_stmt;
    }
    if (orderby_for_groupby != nullptr) {
        delete orderby_for_groupby;
    }
    if (orderby_stmt != nullptr) {
        delete orderby_stmt;
    }
    if (having_stmt != nullptr) {
        delete having_stmt;
    }
    return rc;
}
