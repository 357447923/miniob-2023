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

#include <regex>
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/group_by_stmt.h"
#include "sql/stmt/orderby_stmt.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

SelectStmt::~SelectStmt()
{
  if (nullptr != filter_stmt_) {
    delete filter_stmt_;
    filter_stmt_ = nullptr;
  }
  if (nullptr != inner_join_filter_stmt_) {
    delete inner_join_filter_stmt_;
    inner_join_filter_stmt_ = nullptr;
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

static void wildcard_fields(Table *table, std::vector<Expression *> &field_metas)
{
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = table_meta.sys_field_num(); i < field_num; i++) {
    field_metas.push_back(new FieldExpr(table, table_meta.field(i)));
  }
}

static void clean_expr_when_fail(std::vector<Expression *> &exprs) {
  for (auto expr : exprs) {
    delete expr;
  }
  exprs.clear();
}

RC SelectStmt::create(Db *db, const SelectSqlNode &select_sql, Stmt *&stmt)
{
  if (nullptr == db) {
    LOG_WARN("invalid argument. db is null");
    return RC::INVALID_ARGUMENT;
  }

  // collect tables in `from` statement
  std::vector<Table *> tables;
  std::unordered_map<std::string, Table *> table_map;
  // 建立表名和表的映射关系
  for (size_t i = 0; i < select_sql.relations.size(); i++) {
    const char *table_name = select_sql.relations[i].c_str();
    if (common::is_blank(table_name)) {
      LOG_WARN("invalid argument. relation name is null. index=%d", i);
      return RC::INVALID_ARGUMENT;
    }

    Table *table = db->find_table(table_name);
    if (nullptr == table) {
      LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }

    tables.push_back(table);
    table_map.insert(std::pair<std::string, Table *>(table_name, table));
  }

  // collect query fields in `select` statement
  // TODO 目前只处理了单表且不带表名的情况
  std::vector<Expression *> query_expressions;
  bool contains_aggr_func = false;
  for (int i = static_cast<int>(select_sql.attributes.size()) - 1; i >= 0; i--) {
    const RelAttrSqlNode &relation_attr = select_sql.attributes[i];
    if (common::is_blank(relation_attr.relation_name.c_str()) &&
        0 == strcmp(relation_attr.attribute_name.c_str(), "*")) {
      if (relation_attr.type == AggFuncType::FUNC_NONE) {
        for (Table *table : tables) {
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
      const char *table_name; const char *field_name;

      if (relation_attr.expression == nullptr) {
        table_name = relation_attr.relation_name.c_str();
        field_name = relation_attr.attribute_name.c_str();
      }else {
        // 这里仅需考虑算数表达式(目前还没有除了表达式和字段之外的语法)
        ArithmeticExpr *arithmetic_expr = (ArithmeticExpr *)relation_attr.expression;
        ArithmeticExpr::find_field_need(table_map, arithmetic_expr);
        if (arithmetic_expr->contains_aggr()) {
          contains_aggr_func = true;
        }
        query_expressions.emplace_back(relation_attr.expression);
        continue;
      }

      if (0 == strcmp(table_name, "*")) {
        if (0 != strcmp(field_name, "*")) {
          LOG_WARN("invalid field name while table is *. attr=%s", field_name);
          clean_expr_when_fail(query_expressions);
          return RC::SCHEMA_FIELD_MISSING;
        }
        for (Table *table : tables) {
          wildcard_fields(table, query_expressions);
        }
      } else {
        auto iter = table_map.find(table_name);
        if (iter == table_map.end()) {
          LOG_WARN("no such table in from list: %s", table_name);
          clean_expr_when_fail(query_expressions);
          return RC::SCHEMA_FIELD_MISSING;
        }

        Table *table = iter->second;
        if (0 == strcmp(field_name, "*")) {
          wildcard_fields(table, query_expressions);
        } else {
          const FieldMeta *field_meta = table->table_meta().field(field_name);
          if (nullptr == field_meta) {
            LOG_WARN("no such field. field=%s.%s.%s", db->name(), table->name(), field_name);
            clean_expr_when_fail(query_expressions);
            return RC::SCHEMA_FIELD_MISSING;
          }
          if (relation_attr.type == FUNC_NONE) {
            query_expressions.emplace_back(new FieldExpr(table, field_meta));
          }else {
            contains_aggr_func = true;
            query_expressions.emplace_back(new AggrFuncExpr(relation_attr.type, new FieldExpr(table, field_meta)));
          }
        }
      }
    } else {
      // select id from xxx(查询时字段没写表名的情况，miniob中不考虑select id from x1, x2中id只有一个表中存在的情况)
      if (tables.size() != 1) {
        if (!relation_attr.expression) {
          LOG_ERROR("select schema should follow 'table.field' in miniob");
          return RC::SQL_SYNTAX;
        }
        if (relation_attr.expression->type() != ExprType::ARITHMETIC) {
          LOG_WARN("invalid. I do not know the attr's table. attr=%s", relation_attr.attribute_name.c_str());
          clean_expr_when_fail(query_expressions);
          return RC::SCHEMA_FIELD_MISSING;
        }
        // 没写表名的时候也可能是select 1+1 这种情况
        ArithmeticExpr *arithmetic_expr = (ArithmeticExpr *)(relation_attr.expression);
        ArithmeticExpr::find_field_need(table_map, arithmetic_expr);
        if (arithmetic_expr->contains_aggr()) {
          contains_aggr_func = true;
        }
        query_expressions.emplace_back(relation_attr.expression);
        continue;
      }

      Table *table = tables[0];
      const FieldMeta *field_meta = nullptr;
      
      if (relation_attr.expression != nullptr) {
        if (relation_attr.expression->type() == ExprType::ARITHMETIC) {
          // 填充算术表达式中的字段
          ArithmeticExpr *arithmetic_expr = (ArithmeticExpr *)relation_attr.expression;
          ArithmeticExpr::find_field_need(table, arithmetic_expr);
          if (arithmetic_expr->contains_aggr()) {
            contains_aggr_func = true;
          }
          query_expressions.emplace_back(relation_attr.expression);
        }
        // 其他的类型暂时不处理，因为目前relation_attr的expression属性有值的情况有且仅有其为算数表达式
        continue;
      }
      if (relation_attr.type == FUNC_NONE) {
        field_meta = table->table_meta().field(relation_attr.attribute_name.c_str());
        if (field_meta == nullptr) {
          LOG_ERROR("Can't find field:'%s'", relation_attr.attribute_name.c_str());
          clean_expr_when_fail(query_expressions);
          return RC::SCHEMA_FIELD_NOT_EXIST;
        }
        query_expressions.push_back(new FieldExpr(table, field_meta, FUNC_NONE));
      }else {
        // int len = strlen(AGGR_FUNC_TYPE_NAME[relation_attr.type]);
        const char *field_name = relation_attr.attribute_name.c_str();
        // 检查field_name是否符合: 仅有一个"*"或为纯数字
        if (std::regex_match(field_name, std::regex("^(\\*|\\d+)$"))) {
          // 聚合函数中只有COUNT(*)，其他不能用*
          if (relation_attr.type != FUNC_COUNT) {
            LOG_INFO("func: %s can't receive '*' as param", AGGR_FUNC_TYPE_NAME[relation_attr.type]);
            clean_expr_when_fail(query_expressions);
            return RC::SQL_SYNTAX;
          }
          contains_aggr_func = true;
          query_expressions.push_back(new AggrFuncExpr(relation_attr.type, new ValueExpr(Value(field_name))));
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
        Expression *aggr_expr = new AggrFuncExpr(relation_attr.type, new FieldExpr(table, field_meta, relation_attr.type));
        query_expressions.push_back(aggr_expr);
      }
    }
  }

  Table *default_table = nullptr;
  if (tables.size() == 1) {
    default_table = tables[0];
  }
  
  FilterStmt *inner_join_filter_stmt = nullptr;
  FilterStmt *filter_stmt = nullptr;
  OrderByStmt *orderby_stmt = nullptr;
  GroupByStmt *groupby_stmt = nullptr;
  OrderByStmt *orderby_for_groupby = nullptr;
  HavingStmt *having_stmt = nullptr;
  SelectStmt *select_stmt = nullptr;


  // create filter statement in `where` statement
  RC rc = FilterStmt::create(db,
      default_table,
      &table_map,
      select_sql.conditions.data(),
      static_cast<int>(select_sql.conditions.size()),
      filter_stmt);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot construct filter stmt");
    goto err_handler;
  }
  // 构造inner_join_filter_stmt
   rc = FilterStmt::create(db,
                              default_table,
                              &table_map,
                              select_sql.join_conditions.data(),
                              static_cast<int>(select_sql.join_conditions.size()),
                              inner_join_filter_stmt); 
  if (rc != RC::SUCCESS) {
      LOG_WARN("cannot construct inner join filter stmt");
      goto err_handler;
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
  

  // 所有前置工作完成，开始构造SelectStmt
  select_stmt = new SelectStmt();
  select_stmt->tables_.swap(tables);
  select_stmt->query_expressions_.swap(query_expressions);
  select_stmt->filter_stmt_ = filter_stmt;
  select_stmt->inner_join_filter_stmt_ = inner_join_filter_stmt;
  select_stmt->groupby_stmt_ = groupby_stmt;
  select_stmt->orderby_for_groupby_ = orderby_for_groupby;
  select_stmt->orderby_stmt_ = orderby_stmt;
  select_stmt->having_stmt_ = having_stmt;
  stmt = select_stmt;
  return RC::SUCCESS;

err_handler:
  clean_expr_when_fail(query_expressions);
  if (inner_join_filter_stmt != nullptr) {
    delete inner_join_filter_stmt;
  }
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
