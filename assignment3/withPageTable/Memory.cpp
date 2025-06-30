#include "Memory.hpp"


Memory::Memory(Config & cf){
    nextAvailableFrame = 0; 
    pageTables.resize(cf.pageTableLevels);
} 

void Memory::setPageTableEntry(uint32_t level, uint32_t index,const PageTableEntry& entry){
    pageTables[level][index] = entry;
}

PageTableEntry Memory::getPageTableEntry(uint32_t level, uint32_t index) const{
    return pageTables[level].at(index);
}