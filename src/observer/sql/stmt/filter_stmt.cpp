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
// Created by Wangyunlai on 2022/5/22.
//

#include "common/rc.h"
#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "common/date.h"
#include "common/typecast.h"
#include "sql/stmt/select_stmt.h"

FilterStmt::~FilterStmt()
{
  for (FilterUnit *unit : filter_units_) {
    delete unit;
  }
  filter_units_.clear();
}

RC FilterStmt::create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const ConditionSqlNode *conditions, int condition_num, FilterStmt *&stmt)
{
  RC rc = RC::SUCCESS;
  stmt = nullptr;

  FilterStmt *tmp_stmt = new FilterStmt();
  for (int i = 0; i < condition_num; i++) {
    FilterUnit *filter_unit = nullptr;
    rc = create_filter_unit(db, default_table, tables, conditions[i], filter_unit);
    if (rc != RC::SUCCESS) {
      delete tmp_stmt;
      LOG_WARN("failed to create filter unit. condition index=%d", i);
      return rc;
    }
    tmp_stmt->filter_units_.push_back(filter_unit);
  }

  stmt = tmp_stmt;
  return rc;
}

RC get_table_and_field(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const RelAttrSqlNode &attr, Table *&table, const FieldMeta *&field)
{
  if (common::is_blank(attr.relation_name.c_str())) {
    table = default_table;
  } else if (nullptr != tables) {
    auto iter = tables->find(attr.relation_name);
    if (iter != tables->end()) {
      table = iter->second;
    }
  } else {
    table = db->find_table(attr.relation_name.c_str());
  }
  if (nullptr == table) {
    LOG_WARN("No such table: attr.relation_name: %s", attr.relation_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  field = table->table_meta().field(attr.attribute_name.c_str());
  if (nullptr == field) {
    LOG_WARN("no such field in table: table %s, field %s", table->name(), attr.attribute_name.c_str());
    table = nullptr;
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  return RC::SUCCESS;
}

/**
 * 判断子查询是否合法
 */
static bool judge_subquery_correctness(SelectStmt *select_stmt) {
  int query_size = select_stmt->query_expressions().size();
  if (query_size != 1) {
    return false;
  }
  return true;
}

/**
 * 初始化filter中的非value
 */
static RC filter_attr_init(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables, 
    const RelAttrSqlNode &attr, FilterObj &filter_obj) {
  if (attr.expression != nullptr) {
      // 尝试获取值，在这里能获取到值的就直接把filter转为值，减少后续重复计算
      Value val;
      RC rc = attr.expression
              ->try_get_value(const_cast<Value &>(val));
      if (rc == RC::SUCCESS) {
        filter_obj.init_value(val);
        return rc;
      }
      switch (attr.expression->type()) {
        case ExprType::ARITHMETIC: {
          // 如果是计算表达式, 则至少有一侧必为field。
          ArithmeticExpr *expr = static_cast<ArithmeticExpr *>(attr.expression);
          if (default_table == nullptr) {
            // 处理多表
            ArithmeticExpr::find_field_need(*tables, expr);
          }else {
            // 处理单表
            ArithmeticExpr::find_field_need(default_table, expr);
          }
        }break;
        
        case ExprType::SUBLIST: {
          
        }break;

        default: {
          LOG_ERROR("Unimplement this expression type:%d .", attr.expression->type());
          return RC::UNIMPLENMENT;
        }
      }     
      filter_obj.init_attr(attr.expression);
    }else if (attr.sub_query != nullptr) {
      SubQueryExpr *expr = new SubQueryExpr();
      Stmt *stmt = nullptr;
      RC rc = SelectStmt::create(db, *attr.sub_query, stmt);
      if (rc != RC::SUCCESS) {
        delete expr;
        LOG_ERROR("selectStmt create error in create subquery");
        return rc;
      }
      SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);
      // 判断生成的SelectStmt是否为合法的子查询规范, 也可以修改语法解析, 推荐修改语法解析
      if (!judge_subquery_correctness(select_stmt)) {
        delete expr;
        delete select_stmt;
        select_stmt = nullptr;
      }
      expr->set_stmt(select_stmt);
      filter_obj.init_attr(expr);
    } else {
      Table *table = nullptr;
      const FieldMeta *field = nullptr;
      RC rc = get_table_and_field(db, default_table, tables, attr, table, field);
      if (rc != RC::SUCCESS) {
        LOG_WARN("cannot find attr");
        return rc;
      }
      filter_obj.init_attr(Field(table, field));
    }
    return RC::SUCCESS;
}

RC FilterStmt::create_filter_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
    const ConditionSqlNode &condition, FilterUnit *&filter_unit)
{
  RC rc = RC::SUCCESS;

  CompOp comp = condition.comp;
  if (comp < EQUAL_TO || comp > NOT_LIKE_OP) {
    LOG_WARN("invalid compare operator : %d", comp);
    return RC::INVALID_ARGUMENT;
  }

  filter_unit = new FilterUnit;
  FilterObj *filter_obj_left = new FilterObj;
  FilterObj *filter_obj_right = new FilterObj;

  filter_unit->set_left(filter_obj_left);
  filter_unit->set_comp(comp);
  filter_unit->set_right(filter_obj_right);

  // 处理左边
  if (!condition.left_is_attr) {
    filter_obj_left->init_value(condition.left_value);
  }else {
    // 初始化左边的表达式属性
    rc = filter_attr_init(db, default_table, tables, condition.left_attr, *filter_obj_left);
    // 处理错误
    if (rc != RC::SUCCESS) {
      delete filter_unit;
      return rc;
    }
  }

  // 处理右边
  if (!condition.right_is_attr) {
    filter_obj_right->init_value(condition.right_value);
  }else {
    // 初始化右边的表达式属性
    rc = filter_attr_init(db, default_table, tables, condition.right_attr, *filter_obj_right);
    // 处理错误
    if (rc != RC::SUCCESS) {
      delete filter_unit;
      return rc;
    }
  }

  // TODO 尝试学着上面进行抽取?
  // TODO 检查两个类型是否能够比较, 并且如果有一边是值，则改为对应类型
  if (filter_obj_left->is_attr && filter_obj_right->is_attr) {
    Expression *left_expr = filter_obj_left->expression;
    Expression *right_expr = filter_obj_right->expression;
    // TODO 检查两个类型是否能够比较
    // 思考一下如何知道表达式的返回值类型

  }else if (filter_obj_left->is_attr && !filter_obj_right->is_attr) {
    Expression *left_expr = filter_obj_left->expression;
    // 目前只处理表达式为Field的情况
    if (left_expr->type() == ExprType::FIELD) {
      Field &field = ((FieldExpr *) left_expr)->field();
      Value &right_value = filter_obj_right->value;
      // TODO 改为有一定复杂规则的类型转换
      if (field.attr_type() != right_value.attr_type()) {
        if (right_value.attr_type() == CHARS && field.attr_type() == DATES) {
          rc = common::type_cast(right_value, DATES);
        }
      }
    }
  }else if (filter_obj_right->is_attr && !filter_obj_left->is_attr) {
    Expression *right_expression = filter_obj_right->expression;
    Value &left_value = filter_obj_left->value;
    // 目前只处理表达式为Field的情况
    if (right_expression->type() == ExprType::FIELD) {
      // TODO 改为有一定复杂规则的类型转换
      Field &right_field = ((FieldExpr*)right_expression)->field();
      if (right_field.attr_type() != left_value.attr_type()) {
        if (left_value.attr_type() == CHARS && right_field.attr_type() == DATES) {
          rc = common::type_cast(left_value, DATES);
        }
      }
    }
  }else {
    Value &left_value = filter_obj_left->value;
    Value &right_value = filter_obj_right->value;
    // TODO 改为有一定复杂规则的类型转换
    AttrType left_type = left_value.attr_type();
    AttrType right_type = right_value.attr_type();
    if (left_type != right_type) {
      if (left_type == CHARS && right_type == DATES) {
        rc = common::type_cast(left_value, right_type);
      }else if (right_type == CHARS && left_type == DATES) {
        rc = common::type_cast(right_value, left_type);
      }
    }
  }


  /*
  // 类型转换可以枚举一下左边是值，右边是值，两边都是属性的情况
  if (!condition.left_is_attr && !condition.right_is_attr) {
    // 对unit左边进行初始化
    FilterObj filter_obj_left;
    filter_obj_left.init_value(condition.left_value);
    filter_unit->set_left(filter_obj_left);
    // 对unit右边进行初始化
    FilterObj filter_obj_right;
    filter_obj_right.init_value(condition.right_value);
    filter_unit->set_right(filter_obj_right);
    filter_unit->set_comp(comp);
    return RC::SUCCESS;
  }
  
  if (condition.left_is_attr) {
    if (condition.left_attr.expression != nullptr) {
      FilterObj filter_obj_left;
      condition.left_attr.expression->try_get_value(const_cast<Value &>(condition.left_value));
      filter_obj_left.init_value(condition.left_value);
      filter_unit->set_left(filter_obj_left);
    }else {
      Table *table = nullptr;
      const FieldMeta *field = nullptr;
      rc = get_table_and_field(db, default_table, tables, condition.left_attr, table, field);
      if (rc != RC::SUCCESS) {
        LOG_WARN("cannot find attr");
        return rc;
      }
      FilterObj filter_obj;
      filter_obj.init_attr(Field(table, field));
      filter_unit->set_left(filter_obj);
    }
  } else {
    FilterObj filter_obj;
    filter_obj.init_value(condition.left_value);
    filter_unit->set_left(filter_obj);
  }

  if (condition.right_is_attr) {
    if (condition.right_attr.expression != nullptr) {
      FilterObj filter_obj_right;
      condition.right_attr.expression->try_get_value(const_cast<Value &>(condition.right_value));
      filter_obj_right.init_value(condition.right_value);
      filter_unit->set_left(filter_obj_right);
    }else {
      Table *table = nullptr;
      const FieldMeta *field = nullptr;
      rc = get_table_and_field(db, default_table, tables, condition.right_attr, table, field);
      if (rc != RC::SUCCESS) {
        LOG_WARN("cannot find attr");
        return rc;
      }
      FilterObj filter_obj;
      filter_obj.init_attr(Field(table, field));
      filter_unit->set_right(filter_obj);
    }
  } else {
    // 获取对应的字段元数据
    Table *table = nullptr;
    const FieldMeta *field = nullptr;
    rc = get_table_and_field(db, default_table, tables, condition.left_attr, table, field);
    if (rc != RC::SUCCESS) {
      LOG_WARN("cannot find attr");
      return rc;
    }
    // 仅有非NULL类型的值才有数据转换的必要
    if (condition.right_value.attr_type() != NULLS) {
      Value& value = const_cast<Value&>(condition.right_value);
      // 进行类型转换
      rc = common::type_cast(value, field->type());
      if (rc != RC::SUCCESS) {
        LOG_WARN("Str: %s, can't be convert to date", value.data());
        return rc;
      }
    }

    FilterObj filter_obj;
    filter_obj.init_value(condition.right_value);
    filter_unit->set_right(filter_obj);
  }
  */

  return rc;
}
