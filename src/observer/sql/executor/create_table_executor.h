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
// Created by Wangyunlai on 2023/6/13.
//

#pragma once

#include "common/rc.h"

class SQLStageEvent;
class CreateTableSelectPhysicalOperator;

/**
 * @brief 创建表的执行器
 * @ingroup Executor
 */
class CreateTableExecutor
{
public:
  CreateTableExecutor() = default;
  virtual ~CreateTableExecutor() = default;

  RC execute(SQLStageEvent *sql_event);
  /**
   * 通过表创建物理算子来创建表
   * @details 必须在表创建物理算子open之后才能调用
   */
  RC execute(CreateTableSelectPhysicalOperator *oper);
};