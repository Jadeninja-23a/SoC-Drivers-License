#include "Memory.hpp"

Memory::Memory(Config & cf){
    nextAvailableFrame = 0;
}

void Memory::setPageTableEntry(uint32_t level, uint32_t index, const PageTableEntry& entry){
    memory_contents[index] = entry.physicalFrame;
}

uint64_t Memory::getPageTableEntry(uint32_t level, uint32_t index) const {
    return memory_contents.at(index);
}
