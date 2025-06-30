#pragma once
#include <vector>
#include <cstdint>

struct Config {
    uint64_t CR3;
    int addressWidth;
    int pageSize;
    int pageTableLevels;
    std::vector<int> bitsPerLevel;
    uint64_t systemMemory;
};
