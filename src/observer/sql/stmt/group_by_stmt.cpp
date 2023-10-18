#include "sql/stmt/group_by_stmt.h"
#include "sql/utils/field_utils.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "common/rc.h"
#include "common/log/log.h"
#include "common/lang/string.h"

GroupByStmt::~GroupByStmt() {
  for (GroupByUnit *unit : groupby_units_) {
    delete unit;
  }
  groupby_units_.clear();
}

RC GroupByStmt::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const RelAttrSqlNode *groupby, int group_num, GroupByStmt *&stmt) {
  if (group_num < 0 || (group_num > 0 && groupby->type != FUNC_NONE)) {
    return RC::SQL_SYNTAX;
  }
  GroupByStmt *group_by_stmt = new GroupByStmt;
  for (int i = 0; i < group_num; i++) {
    GroupByUnit *unit = nullptr;
    RC rc = create_groupby_unit(db, default_table, tables, *(groupby + i), unit);
    if (RC::SUCCESS != rc) {
      delete group_by_stmt;
      return rc;
    }
    group_by_stmt->groupby_units_.emplace_back(unit);
  }
  
  stmt = group_by_stmt;
  return RC::SUCCESS;
}

RC GroupByStmt::create_groupby_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const RelAttrSqlNode &groupby, GroupByUnit *&groupby_unit) {
  GroupByUnit *unit = new GroupByUnit;
  const Table *table = nullptr;
  const FieldMeta *field_meta = nullptr;
  RC rc = common::field::get_table_and_field(db, default_table, tables, groupby, table, field_meta);
  if (rc != RC::SUCCESS) {
    delete unit;
    return rc;
  }
  unit->set_expr(new FieldExpr(table, field_meta));
  groupby_unit = unit;

  return RC::SUCCESS;
}