#include "sql/stmt/orderby_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "common/rc.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/utils/field_utils.h"

OrderByStmt::~OrderByStmt() {
  for (OrderByUnit *unit : orderby_units_) {
    delete unit;
  }
  orderby_units_.clear();
}

RC OrderByStmt::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const OrderSqlNode *orderbys, int orderby_num, OrderByStmt *&stmt) {
  if (orderby_num < 0) {
    return RC::UNIMPLENMENT;
  }
  OrderByStmt *orderby_stmt = new OrderByStmt;
  for(int i = 0; i < orderby_num; i++) {
    OrderByUnit *unit = nullptr;
    RC rc = create_orderby_unit(db, default_table, tables, *(orderbys + i), unit);
    if (RC::SUCCESS != rc) {
      // 所有已经添加入orderby_stmt中的unit都被orderby_stmt进行了管理, 故不需要考虑unit的释放
      delete orderby_stmt;
      return rc;
    }
    orderby_stmt->orderby_units_.emplace_back(unit);
  }
  
  stmt = orderby_stmt;
  return RC::SUCCESS;
}

RC OrderByStmt::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const RelAttrSqlNode *groupbys, int groupby_num, OrderByStmt *&stmt) {
  RC rc = RC::SUCCESS;
  OrderByStmt *tmp_stmt = new OrderByStmt();
  for (int i = 0; i < groupby_num; i++) {
    OrderByUnit *orderby_unit = nullptr;
    rc = create_orderby_unit(db, default_table, tables, groupbys[i], orderby_unit);
    if (rc != RC::SUCCESS) {
      delete tmp_stmt;
      LOG_WARN("failed to create filter unit. condition index=%d", i);
      return rc;
    }
    tmp_stmt->orderby_units_.push_back(orderby_unit);
  }

  stmt = tmp_stmt;
  return rc;
}

RC OrderByStmt::create_orderby_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const RelAttrSqlNode &groupby, OrderByUnit *&orderby_unit) {
  RC rc = RC::SUCCESS;

  bool is_asc = true;

  Expression *expr = nullptr;
  const Table *table = nullptr;
  const FieldMeta *field = nullptr;
  rc = common::field::get_table_and_field(db, default_table, tables, groupby, table, field);
  if (rc != RC::SUCCESS) {
    LOG_WARN("cannot find attr");
    return rc;
  }
  expr = new FieldExpr(table, field);
  orderby_unit = new OrderByUnit;
  orderby_unit->set_sort_type(is_asc);
  orderby_unit->set_expr(expr);
  return rc;
}

RC OrderByStmt::create_orderby_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const OrderSqlNode &orderby, OrderByUnit *&orderby_unit) {
  OrderByUnit *unit = new OrderByUnit;
  const Table *table;
  const FieldMeta *field_meta;
  RC rc = common::field::get_table_and_field(db, default_table, tables, orderby.attribute, table, field_meta);
  if (rc != RC::SUCCESS) {
    delete unit;
    return rc;
  }
  unit->set_sort_type(orderby.is_asc);
  unit->set_expr(new FieldExpr(table, field_meta));
  orderby_unit = unit;
  
  return RC::SUCCESS;
}