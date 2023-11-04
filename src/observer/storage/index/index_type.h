#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <locale>

enum class IndexType {
    NORMAL_IDX,
    UNIQUE_IDX,
    UNKNOWN
};

IndexType stringToIndex(const std::string& str);

std::string indexToString(IndexType index);

std::string toUpperCase(const std::string& str);