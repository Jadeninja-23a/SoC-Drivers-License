#include "MMU.hpp"
#include <cmath>

MMU::MMU(Memory& memory, const Config& cfg):mem(memory), config(cfg){}

uint64_t MMU::translate(uint64_t vaddr){
    uint32_t offset =  (config.pageSize - 1) & vaddr;
    int offsetBits = std::log2(config.pageSize);
    vaddr = vaddr >> offsetBits;
    uint64_t paddr = offset;
    int sumBits = offsetBits;

    for(int i = config.pageTableLevels-1; i>=0; i--){
        uint32_t mask = ((1 << config.bitsPerLevel[i]) - 1);
        uint32_t index = mask & vaddr;
        vaddr = vaddr >> config.bitsPerLevel[i];
        try{
            paddr = paddr | (mem.getPageTableEntry(i, index).physicalFrame << sumBits);
        }catch(const std::exception & e){
            bool user = (i==config.pageTableLevels-1) ? true:false;
            struct PageTableEntry p1 = {
                .physicalFrame = mem.nextAvailableFrame >> offsetBits,
                .present = true,
                .user = user ,
                .write = true
            };
            mem.setPageTableEntry(i, index, p1);
            mem.nextAvailableFrame += config.pageSize;
            paddr = paddr | (mem.getPageTableEntry(i, index).physicalFrame << sumBits);
        }
        sumBits += config.bitsPerLevel[i];
    }
    return paddr;
}
void MMU::storeVal(uint64_t vaddr, uint64_t val){
    uint64_t paddr = translate(vaddr);

    mem.memory_contents[paddr] = val;
}
uint64_t MMU::loadVal(uint64_t vaddr){
    uint64_t paddr = translate(vaddr);

    return mem.memory_contents.at(paddr);
}
