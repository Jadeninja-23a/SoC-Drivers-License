#pragma once
#include <vector>

struct Config {
    int addressWidth;
    int pageSize;
    int pageTableLevels;
    std::vector<int> bitsPerLevel;
    uint64_t systemMemory;
};
