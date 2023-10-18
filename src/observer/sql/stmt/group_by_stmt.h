#pragma once

#include <vector>
#include <unordered_map>
#include "sql/parser/parse_defs.h"
#include "sql/stmt/stmt.h"
#include "sql/expr/expression.h"

class Db;
class Table;
class FieldMeta;

class GroupByUnit {
public:
  GroupByUnit() = default;
  ~GroupByUnit() {
    if (expr_) {
      delete expr_;
      expr_ = nullptr;
    }
  }

  void set_expr(Expression *expr) {
    expr_ = expr;
  }

  Expression *expr() const {
    return expr_;
  }

private:
  Expression *expr_ = nullptr;
};

class GroupByStmt {
public:
  GroupByStmt() = default;
  virtual ~GroupByStmt();

  const std::vector<GroupByUnit *>& groupby_units() const {
    return groupby_units_;
  }

  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const RelAttrSqlNode *groupby, int group_num, GroupByStmt *&stmt);

  static RC create_groupby_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const RelAttrSqlNode &groupby, GroupByUnit *&groupby_unit);

private:
  std::vector<GroupByUnit *> groupby_units_;
};

