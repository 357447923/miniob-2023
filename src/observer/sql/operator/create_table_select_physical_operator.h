#pragma once

#include "sql/operator/physical_operator.h"
#include "sql/executor/create_table_executor.h"

class Db;
class CreateTableStmt;

class CreateTableSelectPhysicalOperator : public PhysicalOperator {
public:
  PhysicalOperatorType type() const override {
    return PhysicalOperatorType::CREATE_TABLE;
  }

  virtual RC open(Trx *trx) override;
  virtual RC next() override;
  virtual RC close() override;

  virtual Tuple *current_tuple() override;

  TupleSchema &schema() {
    return schema_;
  }

  void set_create_table_stmt(CreateTableStmt *stmt) {
    create_table_stmt_ = stmt;
  }

  CreateTableStmt *create_table_stmt() {
    return create_table_stmt_;
  }

  void set_db(Db *db) {
    db_ = db;
  }

  Db *db() {
    return db_;
  }

private:
  bool is_first_enter_ = false;
  CreateTableStmt *create_table_stmt_ = nullptr;
  Db *db_ = nullptr;
  TupleSchema schema_;
  CreateTableExecutor executor_;
};