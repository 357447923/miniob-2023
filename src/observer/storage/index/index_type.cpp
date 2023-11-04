#include "storage/index/index_type.h"


// 创建字符串到索引类型的映射

std::unordered_map<std::string, IndexType> stringToIndexMap = {
    {"NORMAL", IndexType::NORMAL_IDX},
    {"UNIQUE", IndexType::UNIQUE_IDX},
};

std::unordered_map<IndexType, std::string> indexToStringMap = {
    {IndexType::NORMAL_IDX, "NORMAL"},
    {IndexType::UNIQUE_IDX, "UNIQUE"},
};

extern std::unordered_map<std::string, IndexType> stringToIndexMap;

IndexType stringToIndex(const std::string& str) {
    std::string indexStr = toUpperCase(str);
    auto it = stringToIndexMap.find(indexStr);
    if (it != stringToIndexMap.end()) {
        return it->second;
    } else {
        // 如果字符串无效，则返回默认值
        return IndexType::UNKNOWN;
    }
}

// 将枚举值映射回字符串
std::string indexToString(IndexType index) {
    auto it = indexToStringMap.find(index);
    if (it != indexToStringMap.end()) {
        return it->second;
    } else {
        // 处理无效的枚举值
        return "UNKNOWN";
    }
}

std::string toUpperCase(const std::string& str) {
    std::string result = str;
    std::locale loc;
    
    for (char& c : result) {
        c = std::toupper(c, loc);
    }
    
    return result;
}
