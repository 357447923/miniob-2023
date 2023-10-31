#pragma once

#include "sql/operator/logical_operator.h"
#include "sql/stmt/create_table_stmt.h"

class Db;

class CreateTableSelectLogicalOperator : public LogicalOperator {
public:
  CreateTableSelectLogicalOperator(CreateTableStmt *create_table_stmt, Db *db) : 
    create_table_stmt_(create_table_stmt), db_(db) {}

  LogicalOperatorType type() const override {
    return LogicalOperatorType::CREATE_TABLE_SELECT;
  }

  CreateTableStmt *create_table_stmt() {
    return create_table_stmt_;
  }

  Db *db() {
    return db_;
  }

private:
  CreateTableStmt *create_table_stmt_;
  Db *db_;
};
