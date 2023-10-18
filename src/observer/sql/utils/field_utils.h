#pragma once
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "common/rc.h"
#include "common/log/log.h"
#include "common/lang/string.h"

namespace common::field {
  RC get_table_and_field(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const RelAttrSqlNode &attr, const Table *&table, const FieldMeta *&field);
}