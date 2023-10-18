#pragma once

#include <vector>
#include <unordered_map>
#include "common/rc.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/stmt.h"
#include "sql/expr/expression.h"

class Db;
class Table;
class FieldMeta;

class OrderByUnit {
public:
  OrderByUnit() = default;

  void set_expr(Expression *expr) {
    expr_ = expr;
  }

  bool sort_type() {
    return is_asc_;
  }

  void set_sort_type(bool is_asc) {
    is_asc_ = is_asc;
  }

  Expression *expr() const {
    return expr_;
  }

private:
  bool is_asc_ = true;
  Expression *expr_ = nullptr;
};

class OrderByStmt {
public:
  OrderByStmt() = default;
  virtual ~OrderByStmt();

  const std::vector<OrderByUnit *>& orderby_units() {
    return orderby_units_;
  }

  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const OrderSqlNode *orderbys, int orderby_num, OrderByStmt *&stmt);
  
  // the func used for creating orderby_stmt for groupby
  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const RelAttrSqlNode *groupbys, int groupby_num, OrderByStmt *&stmt);

  static RC create_orderby_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const OrderSqlNode &orderby, OrderByUnit *&orderby_unit);
  // orderunit for group by
  static RC create_orderby_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const RelAttrSqlNode &groupby, OrderByUnit *&orderby_unit);

private:
  std::vector<OrderByUnit *> orderby_units_;
};