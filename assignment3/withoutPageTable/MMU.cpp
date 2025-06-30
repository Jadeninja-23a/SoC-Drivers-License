#include "MMU.hpp"
#include <cmath>

MMU::MMU(Memory& memory, const Config& cfg):mem(memory), config(cfg) {
    CR3 = cfg.CR3;
}

uint64_t MMU::translate(uint64_t vaddr){
    uint32_t offset = vaddr &(config.pageSize -1);
    int offsetBits = (int)log2(config.pageSize);
    uint32_t paddr = CR3;
    int sumBits = config.addressWidth;
    for(int i = 0; i < config.pageTableLevels; i++){
        sumBits -= config.bitsPerLevel[i];
        uint32_t vpn_mask = (1<<config.bitsPerLevel[i]) -1;
        uint32_t vpn = (vaddr >> sumBits) & vpn_mask;
        PageTableEntry p;
        try{
            p.physicalFrame = mem.getPageTableEntry(i, paddr+vpn);
        } catch(const std::exception & e){
            p.physicalFrame = mem.nextAvailableFrame;
            mem.nextAvailableFrame += config.pageSize;
            mem.setPageTableEntry(i, vpn+paddr, p);
        }
        paddr = p.physicalFrame;
    }
    return (paddr << offsetBits) | offset;
}

void MMU::storeVal(uint64_t vaddr, uint64_t val){
    uint64_t paddr = translate(vaddr);
    mem.memory_contents[paddr] = val;
}

uint64_t MMU::loadVal(uint64_t vaddr){
    uint64_t paddr = translate(vaddr);
    return mem.memory_contents.at(paddr);
}