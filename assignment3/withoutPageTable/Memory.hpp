#pragma once
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "Config.h"

struct PageTableEntry {
    uint64_t physicalFrame;  // has the physical frame number of next level of final address
    bool present; // Is the page for the address allocated?
    bool user;    // Is it a kernel page or user page?
    bool write;   // is it a READ-ONLY page?
};

class Memory {
    friend class MMU; // we want MMU to be able to access the private methods but give access to the user
    public:
        Memory(Config & cf);
    private:

        // These two are memory operations, so they are part of the memory class
        void setPageTableEntry(uint32_t level, uint32_t index, const PageTableEntry& entry); // to simplify we'll assume that you can set your own page table entry
        uint64_t getPageTableEntry(uint32_t level, uint32_t index) const;
        
        std::vector<std::unordered_map<uint32_t, PageTableEntry>> pageTables; // here the index denotes the level of the pagetable
        
        std::unordered_map<uint64_t, uint64_t> memory_contents; // stores the things you want to store

        uint64_t nextAvailableFrame; // This should always be divisible by the page size and would be used for allocation
};  
