#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

enum class IndexType {
    NORMAL_IDX,
    UNIQUE_IDX
};

// extern std::unordered_map<std::string, IndexType> stringToIndexMap;

// IndexType stringToIndex(const std::string& str);

// std::string indexToString(IndexType index);