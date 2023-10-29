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
// Created by Wangyunlai on 2022/07/05.
//

#pragma once

#include <string.h>
#include <memory>
#include <string>
#include <unordered_set>

#include "storage/field/field.h"
#include "sql/parser/value.h"
#include "common/log/log.h"
#include "sql/parser/parse_defs.h"

class Tuple;
class SelectStmt;
class LogicalOperator;
class ProjectLogicalOperator;
class ProjectPhysicalOperator;

/**
 * @defgroup Expression
 * @brief 表达式
 */

/**
 * @brief 表达式类型
 * @ingroup Expression
 */
enum class ExprType 
{
  NONE,
  STAR,         ///< 星号，表示所有字段
  FIELD,        ///< 字段。在实际执行时，根据行数据内容提取对应字段的值
  VALUE,        ///< 常量值
  CAST,         ///< 需要做类型转换的表达式
  COMPARISON,   ///< 需要做比较的表达式
  CONJUNCTION,  ///< 多个表达式使用同一种关系(AND或OR)来联结
  ARITHMETIC,   ///< 算术运算

  ///////////////////////////////////////////////////////

  AGGRFUNCTION, ///< 聚合函数
  FUNC,         ///< 函数(非聚合)
  SUBQUERY,     ///< 子查询
  SUBLIST,      ///< 用于例如 IN (1,2,3) 子句
};

/**
 * @brief 表达式的抽象描述
 * @ingroup Expression
 * @details 在SQL的元素中，任何需要得出值的元素都可以使用表达式来描述
 * 比如获取某个字段的值、比较运算、类型转换
 * 当然还有一些当前没有实现的表达式，比如算术运算。
 *
 * 通常表达式的值，是在真实的算子运算过程中，拿到具体的tuple后
 * 才能计算出来真实的值。但是有些表达式可能就表示某一个固定的
 * 值，比如ValueExpr。
 */
class Expression 
{
public:
  Expression() = default;
  virtual ~Expression() = default;

  /**
   * @brief 根据具体的tuple，来计算当前表达式的值。tuple有可能是一个具体某个表的行数据
   */
  virtual RC get_value(const Tuple &tuple, Value &value) const = 0;

  /**
   * @brief 在没有实际运行的情况下，也就是无法获取tuple的情况下，尝试获取表达式的值
   * @details 有些表达式的值是固定的，比如ValueExpr，这种情况下可以直接获取值
   */
  virtual RC try_get_value(Value &value) const
  {
    return RC::UNIMPLENMENT;
  }

  /**
   * @brief 表达式的类型
   * 可以根据表达式类型来转换为具体的子类
   */
  virtual ExprType type() const = 0;

  /**
   * @brief 表达式值的类型
   * @details 一个表达式运算出结果后，只有一个值
   */
  virtual AttrType value_type() const = 0;

  /**
   * @brief 表达式的名字，比如是字段名称，或者用户在执行SQL语句时输入的内容
   */
  virtual std::string name() const { return name_; }
  virtual void set_name(std::string name) { name_ = name; }

  virtual const char *alias() const {
    return alias_;
  }

  virtual void set_alias(const char *alias) {
    alias_ = alias;
  }

private:
  std::string name_;
  // 最好来说是name_就是alias_,但是此处的name_被我用在了FieldExpr等表达式解析上
  // 所以被迫无奈再加一个alias_的字段
  const char *alias_ = nullptr;
};

/**
 * @brief 字段表达式
 * @ingroup Expression
 */
class FieldExpr : public Expression 
{
public:
  FieldExpr() = default;
  FieldExpr(const Table *table, const FieldMeta *field, const AggFuncType type = FUNC_NONE) : field_(table, field, type)
  {}
  FieldExpr(const Field &field) : field_(field)
  {}

  virtual ~FieldExpr() = default;

  ExprType type() const override { return ExprType::FIELD; }
  AttrType value_type() const override { return field_.attr_type(); }

  AggFuncType func_type() const { return field_.type(); }

  Field &field() { return field_;}

  const Field &field() const { return field_; }

  const char *table_name() const { return field_.table_name(); }

  const char *field_name() const { return field_.field_name(); }

  RC get_value(const Tuple &tuple, Value &value) const override;

private:
  Field field_;
};

/**
 * @brief 常量值表达式
 * @ingroup Expression
 */
class ValueExpr : public Expression 
{
public:
  ValueExpr() = default;
  ValueExpr(const Value &&value) {
    value_ = value;
  }
  explicit ValueExpr(const Value &value) : value_(value)
  {}

  virtual ~ValueExpr() = default;

  RC get_value(const Tuple &tuple, Value &value) const override;
  RC try_get_value(Value &value) const override { value = value_; return RC::SUCCESS; }

  ExprType type() const override { return ExprType::VALUE; }

  AttrType value_type() const override { return value_.attr_type(); }

  void get_value(Value &value) const { value = value_; }

  const Value &get_value() const { return value_; }

private:
  Value value_;
};

/**
 * @brief 类型转换表达式
 * @ingroup Expression
 */
class CastExpr : public Expression 
{
public:
  CastExpr(std::unique_ptr<Expression> child, AttrType cast_type);
  virtual ~CastExpr();

  ExprType type() const override
  {
    return ExprType::CAST;
  }
  RC get_value(const Tuple &tuple, Value &value) const override;

  RC try_get_value(Value &value) const override;

  AttrType value_type() const override { return cast_type_; }

  std::unique_ptr<Expression> &child() { return child_; }

private:
  RC cast(const Value &value, Value &cast_value) const;

private:
  std::unique_ptr<Expression> child_;  ///< 从这个表达式转换
  AttrType cast_type_;  ///< 想要转换成这个类型
};

/**
 * @brief 比较表达式
 * @ingroup Expression
 */
class ComparisonExpr : public Expression 
{
public:
  ComparisonExpr(CompOp comp, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
  virtual ~ComparisonExpr();

  ExprType type() const override { return ExprType::COMPARISON; }

  RC get_value(const Tuple &tuple, Value &value) const override;

  AttrType value_type() const override { return BOOLEANS; }

  CompOp comp() const { return comp_; }

  std::unique_ptr<Expression> &left()  { return left_;  }
  std::unique_ptr<Expression> &right() { return right_; }

  /**
   * 尝试在没有tuple的情况下获取当前表达式的值
   * 在优化的时候，可能会使用到
   */
  RC try_get_value(Value &value) const override;

  /**
   * compare the two tuple cells
   * @param value the result of comparison
   */
  RC compare_value(const Value &left, const Value &right, bool &value) const;

private:
  const CompOp comp_;
  std::unique_ptr<Expression> left_;
  std::unique_ptr<Expression> right_;
};

/**
 * @brief 联结表达式
 * @ingroup Expression
 * 多个表达式使用同一种关系(AND或OR)来联结
 * 当前miniob仅有AND操作
 */
class ConjunctionExpr : public Expression 
{
public:
  enum class Type {
    AND,
    OR,
  };

public:
  ConjunctionExpr(Type type, std::vector<std::unique_ptr<Expression>> &children);
  virtual ~ConjunctionExpr() = default;

  ExprType type() const override { return ExprType::CONJUNCTION; }

  AttrType value_type() const override { return BOOLEANS; }

  RC get_value(const Tuple &tuple, Value &value) const override;

  Type conjunction_type() const { return conjunction_type_; }

  std::vector<std::unique_ptr<Expression>> &children() { return children_; }

private:
  Type conjunction_type_;
  std::vector<std::unique_ptr<Expression>> children_;
};

class AggrFuncExpr;
/**
 * @brief 算术表达式
 * @ingroup Expression
 */
class ArithmeticExpr : public Expression 
{
public:
  enum class Type {
    ADD,
    SUB,
    MUL,
    DIV,
    NEGATIVE,
  };

public:
  ArithmeticExpr(Type type, Expression *left, Expression *right);
  ArithmeticExpr(Type type, std::unique_ptr<Expression> left, std::unique_ptr<Expression> right);
  virtual ~ArithmeticExpr() = default;

  ExprType type() const override { return ExprType::ARITHMETIC; }

  AttrType value_type() const override;

  RC get_value(const Tuple &tuple, Value &value) const override;
  RC try_get_value(Value &value) const override;

  bool contains_aggr() const { return contains_aggr_; }

  Type arithmetic_type() const { return arithmetic_type_; }

  std::unique_ptr<Expression> &left() { return left_; }
  std::unique_ptr<Expression> &right() { return right_; }
  /**
   * 单表查询时的字段表达式填充
   * @return 返回值表示在式子中是否存在聚合函数
   */
  static void find_field_need(const Table *table, ArithmeticExpr *expression);
  /**
   * 多表查询时的字段表达式填充
   * 仅仅支持t.xxx这种, 不能做到识别出字段属于哪个表(对于miniob来说足矣)
   * @return 返回值表示在式子中是否存在聚合函数
   */
  static void find_field_need(const std::unordered_map<std::string, Table *> &table_map, ArithmeticExpr *expression);

  static void find_aggr_func(ArithmeticExpr *expression, std::vector<AggrFuncExpr *> &exprs);

private:
  RC calc_value(const Value &left_value, const Value &right_value, Value &value) const;
  
private:
  bool contains_aggr_ = false;
  Type arithmetic_type_;
  std::unique_ptr<Expression> left_;
  std::unique_ptr<Expression> right_;
};

class AggrFuncExpr : public Expression {
public:
  AggrFuncExpr() = default;
  // 通过这个传入的value, 必须由AggrfuncExpr对象管理
  AggrFuncExpr(AggFuncType type, ValueExpr *value);

  // 通过这个传入的field, 必须由AggrfuncExpr对象管理
  AggrFuncExpr(AggFuncType type, FieldExpr *field);

  ~AggrFuncExpr() {
    if (value_ != nullptr) {
      delete value_;
      value_ = nullptr;
    }
    if (field_ != nullptr) {
      delete field_;
      field_ = nullptr;
    }
  };

  ExprType type() const override {
    return ExprType::AGGRFUNCTION;
  }

  AttrType value_type() const override;
  
  RC get_value(const Tuple &tuple, Value &value) const override;
  RC try_get_value(Value &value) const override;

  FieldExpr *field() {
    return field_;
  }

  ValueExpr *value() {
    return value_;
  }

  AggFuncType aggr_type() {
    return type_;
  }

private:
  AggFuncType type_ = AggFuncType::FUNC_NONE;
  FieldExpr *field_ = nullptr;    // such as count(id), max(id), etc.
  ValueExpr *value_ = nullptr;    // such as count(1), count("xxx"), etc.
};

class FuncExpr : public Expression {
public:
  FuncExpr() = default;
  FuncExpr(FuncType func_type, int param_size, Expression *param1, Expression *param2);
  virtual ~FuncExpr() = default;

  RC get_value(const Tuple &tuple, Value &value) const override;

  ExprType type() const override {
    return ExprType::FUNC;
  }

  AttrType value_type() const override {
    return AttrType::UNDEFINED;
  }

  FuncType func_type() {
    return func_type_;
  }

  std::vector<Expression *> get_params() {
    return params_expr_;
  }

  int get_param_size() {
    return param_size_;
  }

  static RC find_field_need(const Table *table, FuncExpr *func_expr);

  static RC find_field_need(const std::unordered_map<std::string, Table *> &table_map, FuncExpr *func_expr);

  static bool check_if_deps_field(FuncExpr *func_expr);

private:
  RC get_func_length_value(const Tuple &tuple, Value &value) const;

  RC get_func_round_value(const Tuple &tuple, Value &value) const;

  RC get_func_data_format_value(const Tuple &tuple, Value &value) const;

private:
  FuncType func_type_;
  std::vector<Expression *> params_expr_;
  int param_size_;
};

class SubQueryExpr : public Expression {
public:
  SubQueryExpr() = default;

  ~SubQueryExpr();

  RC get_value(const Tuple &tuple, Value &value) const override;
  RC get_value(Value &value) const;

  ExprType type() const override {
    return ExprType::SUBQUERY;
  }

  void set_stmt(SelectStmt *stmt) {
    select_stmt_ = stmt;
  }

  SelectStmt *select_stmt() {
    return select_stmt_;
  }

  void set_sub_query_logical(ProjectLogicalOperator *select_logical) {
    sub_query_logical_ = select_logical;
  }

  ProjectLogicalOperator *sub_query_logical() {
    return sub_query_logical_;
  }

  void set_sub_query_operator(ProjectPhysicalOperator *sub_query_operator) {
    sub_query_operator_ = sub_query_operator;
  }

  const ProjectPhysicalOperator *sub_query_operator() const {
    return sub_query_operator_;
  }

  void set_trx(Trx *trx) {
    trx_ = trx;
  }

  RC open_sub_query() const;
  RC close_sub_query() const;

  AttrType value_type() const override {
    LOG_ERROR("SubQueryExpr.value_type() waitting for implenment");
    return AttrType::UNDEFINED;
  };

private:
  Trx *trx_ = nullptr;
  SelectStmt *select_stmt_ = nullptr;       // 子查询的select语句, 不属于表达式管理
  ProjectLogicalOperator *sub_query_logical_ = nullptr; // 子查询逻辑算子
  ProjectPhysicalOperator *sub_query_operator_ = nullptr;   // 子查询的物理算子, 属于表达式管理(待定)
};

/**
 * IN (1, 2, 3) 表达式 
 */
class ListQueryExpr : public Expression {
public:
  ListQueryExpr(const Value *, int size);

  ListQueryExpr(const std::vector<Value> &values);
  /**
   * 把入参的数据交换到ListQueryExpr对象中
   */
  ListQueryExpr(std::unordered_set<Value, Value> &value_list);

  RC get_value(const Tuple &tuple, Value &value) const override {
    return RC::UNIMPLENMENT;
  }

  ExprType type() const override {
    return ExprType::SUBLIST;
  }

  bool contains(const Value &value) const {
    return value_list_.contains(value);
  }

  const std::unordered_set<Value, Value> &value_list() const {
    return value_list_;
  }

  AttrType value_type() const override {
    return value_list_.empty()? AttrType::UNDEFINED: (value_list_.begin())->attr_type();
  };

private:
  std::unordered_set<Value, Value> value_list_;    // 用于存放数据(IN 的数据)
};