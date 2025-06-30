#pragma once
#include <stdint.h>
#include "Memory.hpp"
#include "Config.h"
#include <string>

struct MMU {
    public:
    MMU(Memory& memory, const Config& cfg);
    void storeVal(uint64_t vaddr, uint64_t val); // we expose only these objects and methods to the user
    uint64_t loadVal(uint64_t vaddr);

    private:
    uint64_t CR3;
    Memory& mem;
    const Config& config;
    uint64_t translate(uint64_t vaddr);
};


