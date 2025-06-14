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
// Created by Wangyunlai 2023/6/27
//

#pragma once

#include <string>
#include "common/rc.h"

#define TEXT_MAX_LEN 65535

/**
 * @brief 属性的类型, 增加属性的时候需要修改value.cpp的ATTR_TYPE_NAME表
 * 
 */
enum AttrType
{
  UNDEFINED,
  CHARS,          ///< 字符串类型
  INTS,           ///< 整数类型(4字节)
  FLOATS,         ///< 浮点数类型(4字节)
  DATES,          ///< 日期类型(4字节)
  NULLS,          ///< NULL类型
  BOOLEANS,       ///< boolean类型，当前不是由parser解析出来的，是程序内部使用的
  TEXTS,          ///< text类型，超长字符串，最多支持65535字节
};

const char *attr_type_to_string(AttrType type);
AttrType attr_type_from_string(const char *s);

/**
 * @brief 属性的值
 * 
 */
class Value 
{
public:
  Value() = default;

  Value(AttrType attr_type, char *data, int length = 4) : attr_type_(attr_type)
  {
    this->set_data(data, length);
  }

  ~Value() = default;
  
  explicit Value(int val);
  explicit Value(float val);
  explicit Value(bool val);
  explicit Value(const char *s, int len = 0);

  Value(const Value &other) = default;
  bool operator!=(const Value &other) const {
    if (attr_type_ != other.attr_type_) {
      return true;
    }
    switch(attr_type_) {
      case INTS: case DATES: {
        return num_value_.int_value_ != other.num_value_.int_value_;
      }break;
      case BOOLEANS: {
        return num_value_.bool_value_ != other.num_value_.bool_value_;
      }break;
      case TEXTS: case CHARS: {
        return str_value_ != other.str_value_;
      }break;
      case FLOATS: {
        // TODO fix Bug of compare float
        return num_value_.float_value_ != other.num_value_.float_value_;
      }break;
    }
    return true;
  }
  
  std::size_t operator()(const Value& obj) const {
    switch (obj.attr_type_)
    {
    case INTS: case DATES: {
      return std::hash<int>{}(obj.get_int());
    }break;
    case FLOATS: {
      return std::hash<float>{}(obj.get_float());
    }break;
    case CHARS: case TEXTS: {
      return std::hash<std::string>{}(obj.get_string());
    }break;
    }
    
    return 0;
  }

  bool operator==(const Value& other) const {
    return compare(other) == RC::LEFT_EQ_ANOTHER;
  }

  Value &operator=(const Value &other) = default;
  
  void set_type(AttrType type)
  {
    this->attr_type_ = type;
  }
  void set_data(char *data, int length);
  void set_data(const char *data, int length)
  {
    this->set_data(const_cast<char *>(data), length);
  }
  void set_int(int val);
  void set_float(float val);
  void set_boolean(bool val);
  void set_null();
  void set_string(const char *s, int len = 0);
  void set_text(const char *s);
  void set_date(int32_t date);
  void set_value(const Value &value);

  bool is_null() const {
    return attr_type_ == NULLS;
  }
  
  std::string to_string() const;

  RC compare(const Value &other) const;
  RC like_compare(const Value &other) const;

  const char *data() const;
  int length() const
  {
    return length_;
  }

  AttrType attr_type() const
  {
    return attr_type_;
  }

public:
  /**
   * 获取对应的值
   * 如果当前的类型与期望获取的类型不符，就会执行转换操作
   */
  int get_int() const;
  float get_float() const;
  std::string get_string() const;
  std::string get_text() const;
  bool get_boolean() const;
  /**
   * @return 当获取失败时，返回-1
   */
  int32_t get_date() const;

private:
  AttrType attr_type_ = UNDEFINED;
  int length_ = 0;

  union {
    int int_value_;
    float float_value_;
    bool bool_value_;
  } num_value_;
  std::string str_value_;
};


RC str_to_target(Value& value, AttrType target);
RC ints_to_target(Value& value, AttrType target);
RC floats_to_target(Value& value, AttrType target);
RC dates_to_target(Value& value, AttrType target);
RC texts_to_target(Value& value, AttrType target);