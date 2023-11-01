
%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  std::cout << "yyerror err msg: " << msg << std::endl;
  std::cout << sql_string << std::endl;
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

static inline void modify_2_negative(Value *value) {
  AttrType type = value->attr_type();
  if (type == AttrType::INTS) {
    value->set_int(value->get_int() * -1);
  }else if(type == AttrType::FLOATS) {
    value->set_float(value->get_float() * -1);
  }
}

void init_relation_sql_node(char *table_name, char *alias, RelationSqlNode &node) {
  node.table = table_name;
  node.alias.reset(alias);
}

%}

%define api.pure full
%define parse.error verbose
/** 启用位置标识 **/
%locations
%lex-param { yyscan_t scanner }
/** 这些定义了在yyparse函数中的参数 **/
%parse-param { const char * sql_string }
%parse-param { ParsedSqlResult * sql_result }
%parse-param { void * scanner }

//标识tokens
%token  SEMICOLON
        CREATE
        DROP
        TABLE
        TABLES
        INDEX
        CALC
        SELECT
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        INT_T
        STRING_T
        FLOAT_T
        DATE_T
        TEXT_T
        null
        IS
        NOT
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        WHERE
        HAVING
        AND
        SET
        INNER
        JOIN
        ON
        GROUP
        ORDER
        BY
        ASC
        DESC
        LOAD
        DATA
        INFILE
        EXPLAIN
        MAX
        MIN
        AVG
        COUNT
        SUM
        LENGTH
        ROUND
        DATE_FORMAT
        EXISTS
        IN
        EQ
        LT
        GT
        LE
        GE
        NE
        LIKE
        AS

/** union 中定义各种数据类型，真实生成的代码也是union类型，所以不能有非POD类型的数据 **/
%union {
  ParsedSqlNode *                   sql_node;
  ConditionSqlNode *                condition;
  OrderSqlNode *                    order_condition;
  SelectSqlNode *                   sub_select;
  Value *                           value;
  enum CompOp                       comp;
  enum FuncType                     func;
  enum AggFuncType                  agg_func;
  RelAttrSqlNode *                  rel_attr;
  std::vector<AttrInfoSqlNode> *    attr_infos;
  AttrInfoSqlNode *                 attr_info;
  Expression *                      expression;
  std::vector<Expression *> *       expression_list;
  std::vector<Value> *              value_list;
  std::vector<std::vector<Value>> * value_lists;
  std::vector<ConditionSqlNode> *   condition_list;
  std::vector<OrderSqlNode> *       order_condition_list;
  std::vector<RelAttrSqlNode> *     rel_attr_list;
  std::vector<std::string> *        relation_list;
  std::vector<SetClauseSqlNode> *   set_clause_list;
  SetClauseSqlNode *                set_clause;
  std::vector<RelAttrSqlNode> *     func_attr_list;
  RelationAndConditionTempList*     relationAndConditionTempList;
  std::vector<std::string> *        index_attr_list;
  char *                            string;
  int                               number;
  float                             floats;
}

%token <number> NUMBER
%token <floats> FLOAT
%token <string> ID
%token <string> SSS
//非终结符

/** type 定义了各种解析后的结果输出的是什么类型。类型对应了 union 中的定义的成员变量名称 **/
%type <number>              type
%type <condition>           condition
%type <order_condition>     order_condition
%type <value>               value
%type <number>              number
%type <comp>                comp_op
%type <func>                func
%type <agg_func>            agg_func
%type <rel_attr>            rel_attr
%type <string>              alias
%type <attr_infos>          attr_def_list
%type <attr_info>           attr_def
%type <value_list>          value_list
%type <value_lists>         value_lists
%type <condition_list>      where
%type <condition_list>      having
%type <condition_list>      condition_list
%type <order_condition_list> order
%type <order_condition_list> order_condition_list
%type <rel_attr_list>       group
%type <rel_attr_list>       group_list
%type <rel_attr>            group_item
%type <rel_attr_list>       select_attr
%type <relation_list>       rel_list
%type <set_clause>          set_clause
%type <set_clause_list>     set_clause_list
%type <sub_select>          sub_query


%type <rel_attr_list>       attr_list
%type <expression>          expression
%type <expression_list>     expression_list
%type <rel_attr_list>       simple_func_list
%type <rel_attr>            simple_func_item
%type <relationAndConditionTempList>       rel_condition_list
%type <index_attr_list>     idx_attr_list
%type <sql_node>            calc_stmt
%type <sql_node>            select_stmt
%type <sql_node>            insert_stmt
%type <sql_node>            update_stmt
%type <sql_node>            delete_stmt
%type <sql_node>            create_table_stmt
%type <sql_node>            drop_table_stmt
%type <sql_node>            show_tables_stmt
%type <sql_node>            desc_table_stmt
%type <sql_node>            create_index_stmt
%type <sql_node>            drop_index_stmt
%type <sql_node>            sync_stmt
%type <sql_node>            begin_stmt
%type <sql_node>            commit_stmt
%type <sql_node>            rollback_stmt
%type <sql_node>            load_data_stmt
%type <sql_node>            explain_stmt
%type <sql_node>            set_variable_stmt
%type <sql_node>            help_stmt
%type <sql_node>            exit_stmt
%type <sql_node>            command_wrapper
// commands should be a list but I use a single command instead
%type <sql_node>            commands

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%%

commands: command_wrapper opt_semicolon  //commands or sqls. parser starts here.
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>($1);
    sql_result->add_sql_node(std::move(sql_node));
  }
  ;

command_wrapper:
    calc_stmt
  | select_stmt
  | insert_stmt
  | update_stmt
  | delete_stmt
  | create_table_stmt
  | drop_table_stmt
  | show_tables_stmt
  | desc_table_stmt
  | create_index_stmt
  | drop_index_stmt
  | sync_stmt
  | begin_stmt
  | commit_stmt
  | rollback_stmt
  | load_data_stmt
  | explain_stmt
  | set_variable_stmt
  | help_stmt
  | exit_stmt
    ;

exit_stmt:      
    EXIT {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      $$ = new ParsedSqlNode(SCF_EXIT);
    };

help_stmt:
    HELP {
      $$ = new ParsedSqlNode(SCF_HELP);
    };

sync_stmt:
    SYNC {
      $$ = new ParsedSqlNode(SCF_SYNC);
    }
    ;

begin_stmt:
    TRX_BEGIN  {
      $$ = new ParsedSqlNode(SCF_BEGIN);
    }
    ;

commit_stmt:
    TRX_COMMIT {
      $$ = new ParsedSqlNode(SCF_COMMIT);
    }
    ;

rollback_stmt:
    TRX_ROLLBACK  {
      $$ = new ParsedSqlNode(SCF_ROLLBACK);
    }
    ;

drop_table_stmt:    /*drop table 语句的语法解析树*/
    DROP TABLE ID {
      $$ = new ParsedSqlNode(SCF_DROP_TABLE);
      $$->drop_table.relation_name = $3;
      free($3);
    };

show_tables_stmt:
    SHOW TABLES {
      $$ = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
    ;

desc_table_stmt:
    DESC ID  {
      $$ = new ParsedSqlNode(SCF_DESC_TABLE);
      $$->desc_table.relation_name = $2;
      free($2);
    }
    ;

create_index_stmt:    /*create index 语句的语法解析树*/
    CREATE INDEX ID ON ID LBRACE ID idx_attr_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = $$->create_index;
      create_index.index_name = $3;
      create_index.relation_name = $5;
      if ($8 != nullptr) {
        create_index.attribute_names.swap(*$8);
        delete $8;
      }
      create_index.attribute_names.push_back($7);
      std::reverse(create_index.attribute_names.begin(), create_index.attribute_names.end());
      create_index.index_type = "NORMAL";
      free($3);
      free($5);
      free($7);
    }
    | CREATE ID INDEX ID ON ID LBRACE ID idx_attr_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = $$->create_index;
      create_index.index_name = $4;
      create_index.relation_name = $6;
      if ($9 != nullptr) {
        create_index.attribute_names.swap(*$9);
        delete $9;
      }
      create_index.attribute_names.push_back($8);
      std::reverse(create_index.attribute_names.begin(), create_index.attribute_names.end());
      create_index.index_type = $2;
      free($2);
      free($4);
      free($6);
      free($8);
    }
    ;

idx_attr_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA ID rel_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<std::string>;
      }
      $$->push_back($2);
      free($2);
    }
    ;

drop_index_stmt:      /*drop index 语句的语法解析树*/
    DROP INDEX ID ON ID
    {
      $$ = new ParsedSqlNode(SCF_DROP_INDEX);
      $$->drop_index.index_name = $3;
      $$->drop_index.relation_name = $5;
      free($3);
      free($5);
    }
    ;
create_table_stmt:    /*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE
    {
      $$ = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = $$->create_table;
      create_table.relation_name = $3;
      free($3);

      std::vector<AttrInfoSqlNode> *src_attrs = $6;

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
      }
      create_table.attr_infos.emplace_back(*$5);
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete $5;
    }
    ;
attr_def_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA attr_def attr_def_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<AttrInfoSqlNode>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    ;
    
attr_def:
    ID type LBRACE number RBRACE 
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      $$->length = $4;
      $$->not_null = true;
      free($1);
    }
    | ID type
    {
      $$ = new AttrInfoSqlNode;
      $$->type = (AttrType)$2;
      $$->name = $1;
      if($$->type == TEXTS) {
        $$->length = 65535;
      } else{
        $$->length = 4;
      }
      $$->not_null = true;
      free($1);
    }
    | attr_def NOT null {
      $$ = $1;
      $$->not_null = true;
    }
    | attr_def null {
      $$ = $1;
      $$->not_null = false;
    }
    ;
number:
    NUMBER {$$ = $1;}
    ;
type:
    INT_T      { $$=INTS; }
    | STRING_T { $$=CHARS; }
    | FLOAT_T  { $$=FLOATS; }
    | DATE_T   { $$=DATES; }
    | TEXT_T   { $$=TEXTS; }
    ;
insert_stmt:        /*insert   语句的语法解析树*/
    INSERT INTO ID VALUES LBRACE value value_list RBRACE value_lists
    {
      $$ = new ParsedSqlNode(SCF_INSERT);
      $$->insertion.relation_name = $3;
      if ($9 != nullptr) {
        $$->insertion.values.swap(*$9);
      }
      if ($7 != nullptr) {
        std::vector<Value> *tmp = $7;
        tmp->emplace_back(*$6);
        $$->insertion.values.emplace_back(*tmp);
        delete $7;
      }else {
        std::vector<Value> tmp = {*$6};
        $$->insertion.values.emplace_back(tmp);
      }
      for (auto &value : $$->insertion.values) {
        std::reverse(value.begin(), value.end());
      }
      std::reverse($$->insertion.values.begin(), $$->insertion.values.end());
      delete $6;
      free($3);
    }
    | INSERT INTO ID VALUES LBRACE '-' value value_list RBRACE value_lists {
      $$ = new ParsedSqlNode(SCF_INSERT);
      $$->insertion.relation_name = $3;
      if ($10 != nullptr) {
        $$->insertion.values.swap(*$10);
      }
      modify_2_negative($7);
      if ($8 != nullptr) {
        std::vector<Value> *tmp = $8;
        tmp->emplace_back(*$7);
        $$->insertion.values.emplace_back(*tmp);
        delete $8;
      }else {
        std::vector<Value> tmp = {*$7};
        $$->insertion.values.emplace_back(tmp);
      }
      for (auto &value : $$->insertion.values) {
        std::reverse(value.begin(), value.end());
      }
      std::reverse($$->insertion.values.begin(), $$->insertion.values.end());
      delete $7;
      free($3);
    }
    ;

value_lists:
  {
    $$ = nullptr;
  }
  | COMMA LBRACE value value_list RBRACE value_lists {
    if ($6 != nullptr) {
      $$ = $6;
    }else {
      $$ = new std::vector<std::vector<Value>>;
    }
    if ($4 != nullptr) {
      std::vector<Value> *tmp = $4;
      tmp->emplace_back(*$3);
      $$->emplace_back(*$4);
      delete $4;
    }else {
      std::vector<Value> tmp = {*$3};
      $$->emplace_back(tmp);
    }
    delete $3;
  }
  | COMMA LBRACE '-' value value_list RBRACE value_lists {
    if ($7 != nullptr) {
      $$ = $7;
    }else {
      $$ = new std::vector<std::vector<Value>>;
    }
    modify_2_negative($4);
    if ($5 != nullptr) {
      std::vector<Value> *tmp = $5;
      tmp->emplace_back(*$4);
      $$->emplace_back(*$5);
      delete $4;
    }else {
      std::vector<Value> tmp = {*$4};
      $$->emplace_back(tmp);
    }
    delete $4;
  }
  ;

value_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA value value_list { 
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Value>;
      }
      $$->emplace_back(*$2);
      delete $2;
    }
    | COMMA '-' value value_list {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<Value>;
      }
      modify_2_negative($3);
      $$->emplace_back(*$3);
      delete $3;
    }
    ;
value:
    NUMBER {
      $$ = new Value((int)$1);
      @$ = @1;
    }
    |FLOAT {
      $$ = new Value((float)$1);
      @$ = @1;
    }
    |SSS {
      char *tmp = common::substr($1,1,strlen($1)-2);
      $$ = new Value(tmp);
      free(tmp);
    }
    |null {
      $$ = new Value();
      $$->set_null();
      @$ = @1;
    }
    ;
    
delete_stmt:    /*  delete 语句的语法解析树*/
    DELETE FROM ID where 
    {
      $$ = new ParsedSqlNode(SCF_DELETE);
      $$->deletion.relation_name = $3;
      if ($4 != nullptr) {
        $$->deletion.conditions.swap(*$4);
        delete $4;
      }
      free($3);
    }
    ;
update_stmt:      /*  update 语句的语法解析树*/
    UPDATE ID SET set_clause_list where 
    {
      $$ = new ParsedSqlNode(SCF_UPDATE);
      $$->update.relation_name = $2;
      if ($4 != nullptr) {
        $$->update.set_clause_list.swap(*$4);
        delete $4;
      }     
      if ($5 != nullptr) {
        $$->update.conditions.swap(*$5);
        delete $5;
      }
      free($2);
    }

set_clause_list: /*set 子句列表*/
    set_clause COMMA set_clause_list
    {
      if($3 != nullptr) {
        $$ = $3;
        $$->push_back(*$1);
      } else {
        $$ = new std::vector<SetClauseSqlNode>;
        $$->push_back(*$1);
      }
    }
    | set_clause
    {
      $$ = new std::vector<SetClauseSqlNode>;
      $$->push_back(*$1);
    }
    ;

set_clause: /* 单个set子句*/
    ID EQ value
    {
      $$ = new SetClauseSqlNode;
      $$->attribute_name_ = $1;
      $$->value_ = *$3;
      free($1);
      delete $3;
    }
    | ID EQ '-' value
    {
      $$ = new SetClauseSqlNode;
      modify_2_negative($4);
      $$->attribute_name_ = $1;
      $$->value_ = *$4;
      free($1);
      delete $4;
    }
    | ID EQ sub_query {
      $$ = new SetClauseSqlNode;
      $$->attribute_name_ = $1;
      $$->sub_query_.reset($3);
      free($1);
    }
    ;


select_stmt:        /*  select 语句的语法解析树*/
    SELECT select_attr FROM ID alias rel_condition_list where group order having
    {
      $$ = new ParsedSqlNode(SCF_SELECT);
      if ($2 != nullptr) {
        $$->selection.attributes.swap(*$2);
        delete $2;
      }
      if ($6 != nullptr) {
        $$->selection.relations.swap($6->_rel_list);
        $$->selection.join_conditions.swap($6->_condition_list);
        std::reverse($$->selection.join_conditions.begin(), $$->selection.join_conditions.end());
        delete $6;
      }
      RelationSqlNode node;
      init_relation_sql_node($4, $5, node);
      $$->selection.relations.push_back(node);
      std::reverse($$->selection.relations.begin(), $$->selection.relations.end());
      if ($7 != nullptr) {
        $$->selection.conditions.swap(*$7);
        delete $7;
      }
      if ($8 != nullptr) {
        std::reverse($8->begin(), $8->end());
        $$->selection.groups.swap(*$8);
        delete $8;
      }
      if ($9 != nullptr) {
        std::reverse($9->begin(), $9->end());
        $$->selection.orders.swap(*$9);
        delete $9;
      }
      if ($10 != nullptr) {
        std::reverse($10->begin(), $10->end());
        $$->selection.havings.swap(*$10);
        delete $10;
      }
      free($4);
    }
    | SELECT simple_func_item alias simple_func_list %prec UMINUS {
      $$ = new ParsedSqlNode(SCF_SELECT);
      $2->alias.reset($3);
      $2->expression->set_alias($3);
      if ($4 != nullptr) {
        $4->push_back(*$2);
        std::reverse($4->begin(), $4->end());
        $$->selection.attributes.swap(*$4);
        delete $4;
      }else {
        $$->selection.attributes.push_back(*$2);
      }
      delete $2;
    }

    ;
calc_stmt:
    CALC expression_list
    {
      $$ = new ParsedSqlNode(SCF_CALC);
      std::reverse($2->begin(), $2->end());
      $$->calc.expressions.swap(*$2);
      delete $2;
    }
    ;

simple_func_item:
  func LBRACE value RBRACE {
    // 这里的ValueExpr找不到合适的地方释放
    ValueExpr *value_expr = new ValueExpr(std::move(*$3));
    value_expr->set_name(value_expr->get_value().to_string());
    FuncExpr *expr = new FuncExpr($1, 1, value_expr, nullptr);
    $$ = new RelAttrSqlNode;
    $$->expression = expr;
    $$->func_type = $1;
    std::string name;
    switch ($1) {
      case FUNC_LENGTH: {
        name += "LENGTH(";
      }break;
      case FUNC_ROUND: {
        name += "ROUND(";
      }break;
      case FUNC_DATE_FORMAT: {
        name += "DATE_FORMAT(";
      }break;
    }
    name += value_expr->name();
    name += ")";
    expr->set_name(name);
    delete $3;
  }
  | func LBRACE value COMMA value RBRACE {
    auto first = new ValueExpr(std::move(*$3));
    first->set_name(first->get_value().to_string());
    auto second = new ValueExpr(std::move(*$5));
    second->set_name(second->get_value().to_string());
    FuncExpr *expr = new FuncExpr($1, 2, first, second);
    $$ = new RelAttrSqlNode;
    $$->expression = expr;
    $$->func_type = $1;
    std::string name;
    switch ($1) {
      case FUNC_LENGTH: {
        name += "LENGTH(";
      }break;
      case FUNC_ROUND: {
        name += "ROUND(";
      }break;
      case FUNC_DATE_FORMAT: {
        name += "DATE_FORMAT(";
      }break;
    }
    name += first->name();
    name += ",";
    name += second->name();
    name += ")";
    expr->set_name(name);
    delete $3;
  }
  ;

simple_func_list: 
  {
    $$ = nullptr;
  }
  | COMMA simple_func_item alias simple_func_list {
    if ($4 != nullptr) {
      $$ = $4;
    }else {
      $$ = new std::vector<RelAttrSqlNode>;
    }
    $2->alias.reset($3);
    $2->expression->set_alias($3);
    $$->push_back(*$2);
    delete $2;
  }
  ;
expression_list:
    expression
    {
      $$ = new std::vector<Expression*>;
      $$->emplace_back($1);
    }
    | expression COMMA expression_list
    {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<Expression *>;
      }
      $$->emplace_back($1);
    }
    ;
expression:
    expression '+' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::ADD, $1, $3, sql_string, &@$);
    }
    | expression '-' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::SUB, $1, $3, sql_string, &@$);
    }
    | expression '*' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::MUL, $1, $3, sql_string, &@$);
    }
    | expression '/' expression {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::DIV, $1, $3, sql_string, &@$);
    }
    | LBRACE expression RBRACE {
      $$ = $2;
      $$->set_name(token_name(sql_string, &@$));
    }
    | '-' expression %prec UMINUS {
      $$ = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, $2, nullptr, sql_string, &@$);
    }
    | value {
      $$ = new ValueExpr(*$1);
      $$->set_name(token_name(sql_string, &@$));
      delete $1;
    }
    | ID {
      $$ = new FieldExpr;
      $$->set_name(std::string($1));
      free($1);
    }
    | ID DOT ID {
      $$ = new FieldExpr;
      $$->set_name(std::string($1).append(".").append(std::string($3)));
      free($3);
      free($1);
    }
    | agg_func LBRACE ID RBRACE {
      FieldExpr *field_expr = new FieldExpr;
      field_expr->set_name(std::string($3));
      $$ = new AggrFuncExpr($1, field_expr);
      free($3);
    }
    | agg_func LBRACE ID DOT ID RBRACE {
      FieldExpr *field_expr = new FieldExpr;
      field_expr->set_name(std::string($3).append(".").append(std::string($5)));
      $$ = new AggrFuncExpr($1, field_expr);
      free($3);
      free($5);
    }
    | agg_func LBRACE '*' RBRACE {
      ValueExpr *expr = new ValueExpr(Value("*", 2));
      expr->set_name("*");
      $$ = new AggrFuncExpr($1, expr);
    }
    | agg_func LBRACE number RBRACE {
      ValueExpr *expr = new ValueExpr(Value($3));
      expr->set_name(std::to_string($3));
      $$ = new AggrFuncExpr($1, expr);
    }
    | func LBRACE expression RBRACE {
      $$ = new FuncExpr($1, 1, $3, nullptr);
      std::string name;
      switch ($1) {
        case FUNC_LENGTH: {
          name += "LENGTH(";
        }break;
        case FUNC_ROUND: {
          name += "ROUND(";
        }break;
        case FUNC_DATE_FORMAT: {
          name += "DATE_FORMAT(";
        }break;
      }
      name += $3->name();
      name += ")";
      $$->set_name(name);
    }
    | func LBRACE expression COMMA expression RBRACE {
      $$ = new FuncExpr($1, 2, $3, $5);
      std::string name;
      switch ($1) {
        case FUNC_LENGTH: {
          name += "LENGTH(";
        }break;
        case FUNC_ROUND: {
          name += "ROUND(";
        }break;
        case FUNC_DATE_FORMAT: {
          name += "DATE_FORMAT(";
        }break;
      }
      name += $3->name();
      name += ", ";
      name += $5->name();
      name += ")";
      $$->set_name(name);
    }
    ;

select_attr:
    '*' {
      $$ = new std::vector<RelAttrSqlNode>;
      RelAttrSqlNode attr;
      attr.relation_name  = "";
      attr.attribute_name = "*";
      $$->emplace_back(attr);
    }
    | rel_attr alias attr_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $1->alias.reset($2);
      $$->emplace_back(*$1);
      delete $1;
    }
    | ID DOT '*' attr_list {
      if ($4 != nullptr) {
        $$ = $4;
      }else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      RelAttrSqlNode node;
      node.relation_name = $1;
      node.attribute_name = "*";
      free($1);
    }
    ;

alias:
    /* empty */
    {
      $$ = nullptr;
    }
    | ID {
      $$ = $1;
    }
    | AS ID %prec UMINUS {
      $$ = $2;
    }
    ;

rel_attr:
    expression {
      $$ = new RelAttrSqlNode;
      if ($1->type() == ExprType::FIELD) {
        std::string field_name = $1->name();
        std::string table_name;
        int idx = field_name.find_first_of(".");
        if (std::string::npos != idx) {
          $$->relation_name = field_name.substr(0, idx);
          $$->attribute_name = field_name.substr(idx + 1, field_name.size() - idx - 1);
        }else {
          $$->attribute_name = $1->name();
        }
        delete $1;
      }else if($1->type() == ExprType::AGGRFUNCTION){
        // TODO 降低冗余
        FieldExpr *expr = ((AggrFuncExpr *)$1)->field();
        if (expr) {
          std::string field_name = expr->name();
          std::string table_name;
          int idx = field_name.find_first_of(".");
          if (std::string::npos != idx) {
            $$->relation_name = field_name.substr(0, idx);
            $$->attribute_name = field_name.substr(idx + 1, field_name.size() - idx - 1);
          }else {
            $$->attribute_name = std::move(field_name);
          }
        }else {
          $$->attribute_name = ((AggrFuncExpr *)$1)->value()->name();
        }
        $$->type = ((AggrFuncExpr *)$1)->aggr_type();
      }else {
        $$->expression = $1;
      }
    }
    // 子查询
    | sub_query {
      $$ = new RelAttrSqlNode;
      $$->sub_query.reset($1);
    }
    // 这个有冲突，当value_list为null时，并且我认为这个不适合放在这里，他只服务于condition才对
    | LBRACE value value_list RBRACE %prec UMINUS {
      $$ = new RelAttrSqlNode;
      if ($3 == nullptr) {
        $3 = new std::vector<Value>;
      }
      $3->push_back(*$2);
      delete $2;
      $$->expression = new ListQueryExpr(*$3);
      delete $3;
    }
    ;

sub_query:
    LBRACE SELECT rel_attr alias FROM ID alias rel_condition_list where group order having RBRACE 
    {
      SelectSqlNode *sub_query = new SelectSqlNode;
      $3->alias.reset($4);
      sub_query->attributes.push_back(*$3);
      if ($8 != nullptr) {
        sub_query->relations.swap($8->_rel_list);
        delete $8;
      }
      RelationSqlNode node;
      init_relation_sql_node($6, $7, node);
      sub_query->relations.push_back(node);
      std::reverse(sub_query->relations.begin(), sub_query->relations.end());
      if ($9 != nullptr) {
        sub_query->conditions.swap(*$9);
        delete $9;
      }
      if ($10 != nullptr) {
        std::reverse($10->begin(), $10->end());
        sub_query->groups.swap(*$10);
        delete $10;
      }
      if ($11 != nullptr) {
        std::reverse($11->begin(), $11->end());
        sub_query->orders.swap(*$11);
        delete $11;
      }
      if ($12 != nullptr) {
        std::reverse($12->begin(), $12->end());
        sub_query->havings.swap(*$12);
        delete $12;
      }
      $$ = sub_query;
      delete $3;
      free($6);
    }

attr_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA rel_attr alias attr_list {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      $2->alias.reset($3);
      $$->emplace_back(*$2);
      delete $2;
    }
    | COMMA ID DOT '*' attr_list {
      if ($5 != nullptr) {
        $$ = $5;
      }else {
        $$ = new std::vector<RelAttrSqlNode>;
      }
      RelAttrSqlNode node;
      node.relation_name = $2;
      node.attribute_name = "*";
      $$->push_back(node);
      free($2);
    }
    ;

rel_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | COMMA ID rel_list {
      if ($3 != nullptr) {
        $$ = $3;
      } else {
        $$ = new std::vector<std::string>;
      }

      $$->push_back($2);
      free($2);
    }
    ;

rel_condition_list:
    /* empty */
    {
      $$ = nullptr;
    }
    /* , t1  */
    | COMMA ID alias rel_condition_list {
      if ($4 != nullptr) {
        $$ = $4;
      } else {
        $$ = new RelationAndConditionTempList;
      }
      RelationSqlNode node;
      init_relation_sql_node($2, $3, node);
      $$->_rel_list.push_back(node);
      free($2);
    }
    // inner join t1 on
    | INNER JOIN ID alias condition_list rel_condition_list{
      if ($6 != nullptr) {
        $$ = $6;
      } else {
        $$ = new RelationAndConditionTempList;
      }
      if($5 != nullptr) {
          $$->_condition_list.push_back(*($5));
          delete $5;
      }
      RelationSqlNode node;
      init_relation_sql_node($3, $4, node);
      $$->_rel_list.push_back(node);
      free($3);
      std::cout << "inner join parse finished" << std::endl;
    }
    ;

where:
    /* empty */
    {
      $$ = nullptr;
    }
    | WHERE condition_list {
      $$ = $2;  
    }
    ;
having:
    {
      $$ = nullptr;
    }
    | HAVING condition_list {
      $$ = $2;
    }
    ;
condition_list:
    /* empty */
    {
      $$ = nullptr;
    }
    | condition {
      $$ = new std::vector<ConditionSqlNode>;
      $$->emplace_back(*$1);
      delete $1;
    }
    | condition AND condition_list {
      $$ = $3;
      $$->emplace_back(*$1);
      delete $1;
    } 
    | ON condition_list {
      if ($2 != nullptr) {
        $$ = $2;
      }else {
        $$ = new std::vector<ConditionSqlNode>;
      }
    }
    ;
condition:
    rel_attr comp_op rel_attr
    {
      $$ = new ConditionSqlNode;
      if ($1->expression == nullptr) {
        $$->left_is_attr = 1;
        $$->left_attr = *$1;
      }else if ($1->expression->type() != ExprType::VALUE) {
        $$->left_is_attr = 1;
        $$->left_attr = *$1;
      }else {
        $$->left_is_attr = 0;
        $1->expression->try_get_value($$->left_value);
      }
      
      if ($3->expression == nullptr) {
        $$->right_is_attr = 1;
        $$->right_attr = *$3;
      }else if ($3->expression->type() != ExprType::VALUE) {
        $$->right_is_attr = 1;
        $$->right_attr = *$3;
      }else {
        $$->right_is_attr = 0;
        $3->expression->try_get_value($$->right_value);
      }
      $$->comp = $2;
      std::cout << "comp type: " << $$->comp << std::endl;
      delete $1;
      delete $3;
    }
    
    ;

group:
  {
    $$ = nullptr;
  }
  | GROUP BY group_item group_list
  {
    if ($4 == nullptr) {
      $$ = new std::vector<RelAttrSqlNode>;
    }else {
      $$ = $4;
    }
    $$->emplace_back(*$3);
    delete $3;
  }
  ;
group_list:
  {
    $$ = nullptr;
  }
  | COMMA group_item group_list {
    if ($3 == nullptr) {
      $$ = new std::vector<RelAttrSqlNode>;
    }else {
      $$ = $3;
    }
    $$->push_back(*$2);
    delete $2;
  }
  ;
group_item:
  ID {
    $$ = new RelAttrSqlNode;
    $$->attribute_name = $1;
    free($1);
  }
  | ID DOT ID {
    $$ = new RelAttrSqlNode;
    $$->relation_name = $1;
    $$->attribute_name = $3;
    free($1);
    free($3);
  }
  ;

order:
  {
    $$ = nullptr;
  }
  | ORDER BY order_condition order_condition_list {
    if ($4 != nullptr) {
      $$ = $4;
    }else {
      $$ = new std::vector<OrderSqlNode>;
    }
    $$->emplace_back(*$3);
    delete $3;
  }
  ;
order_condition_list:
  {
    $$ = nullptr;
  }
  | COMMA order_condition order_condition_list {
    if ($3 != nullptr) {
      $$ = $3;
    }else {
      $$ = new std::vector<OrderSqlNode>;
    }
    $$->emplace_back(*$2);
    delete $2;
  }
  ;

order_condition:
  ID {
    $$ = new OrderSqlNode;
    $$->attribute.attribute_name = $1;
    $$->is_asc = 1;
    free($1);
  }
  | ID DOT ID {
    $$ = new OrderSqlNode;
    $$->attribute.relation_name = $1;
    $$->attribute.attribute_name = $3;
    $$->is_asc = 1;
    free($1);
    free($3);
  }
  | ID ASC {
    $$ = new OrderSqlNode;
    $$->attribute.attribute_name = $1;
    $$->is_asc = 1;
    free($1);
  }
  | ID DESC {
    $$ = new OrderSqlNode;
    $$->attribute.attribute_name = $1;
    $$->is_asc = 0;
    free($1);
  }
  | ID DOT ID ASC {
    $$ = new OrderSqlNode;
    $$->attribute.relation_name = $1;
    $$->attribute.attribute_name = $3;
    $$->is_asc = 1;
    free($1);
    free($3);
  }
  | ID DOT ID DESC {
    $$ = new OrderSqlNode;
    $$->attribute.relation_name = $1;
    $$->attribute.attribute_name = $3;
    $$->is_asc = 0;
    free($1);
    free($3);
  }
agg_func:
      MAX { $$ = FUNC_MAX; }
    | MIN { $$ = FUNC_MIN; }
    | COUNT { $$ = FUNC_COUNT; }
    | AVG { $$ = FUNC_AVG; }
    | SUM { $$ = FUNC_SUM; }

func:
      LENGTH { $$ = FUNC_LENGTH; }
    | ROUND { $$ = FUNC_ROUND; }
    | DATE_FORMAT { $$ = FUNC_DATE_FORMAT; }

comp_op:
      EQ { $$ = EQUAL_TO; }
    | LT { $$ = LESS_THAN; }
    | GT { $$ = GREAT_THAN; }
    | LE { $$ = LESS_EQUAL; }
    | GE { $$ = GREAT_EQUAL; }
    | NE { $$ = NOT_EQUAL; }
    | IS { $$ = IS_NULL; }
    | IS NOT { $$ = NOT_NULL; }
    | EXISTS { $$ = EXISTS_OP; }
    | NOT EXISTS { $$ = NOT_EXISTS_OP; }
    | IN { $$ = IN_OP; }
    | NOT IN { $$ = NOT_IN_OP; }
    | LIKE { $$ = LIKE_OP; }
    | NOT LIKE { $$ = NOT_LIKE_OP; }
    ;

load_data_stmt:
    LOAD DATA INFILE SSS INTO TABLE ID 
    {
      char *tmp_file_name = common::substr($4, 1, strlen($4) - 2);
      
      $$ = new ParsedSqlNode(SCF_LOAD_DATA);
      $$->load_data.relation_name = $7;
      $$->load_data.file_name = tmp_file_name;
      free($7);
      free(tmp_file_name);
    }
    ;

explain_stmt:
    EXPLAIN command_wrapper
    {
      $$ = new ParsedSqlNode(SCF_EXPLAIN);
      $$->explain.sql_node = std::unique_ptr<ParsedSqlNode>($2);
    }
    ;

set_variable_stmt:
    SET ID EQ value
    {
      $$ = new ParsedSqlNode(SCF_SET_VARIABLE);
      $$->set_variable.name  = $2;
      $$->set_variable.value = *$4;
      free($2);
      delete $4;
    }
    | SET ID EQ '-' value
    {
      $$ = new ParsedSqlNode(SCF_SET_VARIABLE);
      $$->set_variable.name = $2;
      $$->set_variable.value = *$5;
      free($2);
      delete $5;
    }
    ;

opt_semicolon: /*empty*/
    | SEMICOLON
    ;
%%
//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
