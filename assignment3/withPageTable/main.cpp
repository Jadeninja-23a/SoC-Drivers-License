#include <iostream>
#include "MMU.cpp"
#include "Memory.cpp"
#include <stdint.h>
#include <cmath>

using namespace std;

int main(){

	uint64_t vaddr;
	uint64_t val;
	
	int n;

	Config cfg = {
		.addressWidth = 32,
		.pageSize = 4096,
		.pageTableLevels = 2,
		.bitsPerLevel = {10, 10},
		.systemMemory = (uint64_t)pow(2, 32)
	};

	Memory mem(cfg);

	MMU mmu(mem, cfg);

	while(true){
		cout << "Enter 1 to set value into memory address\nEnter 2 to load value from memory address\nEnter anything else to quit" << endl;
		cin >> n;
		if(n==1){
			cin >> vaddr >> val;
			mmu.storeVal(vaddr, val);//no safety check so if invalid virtual addres given goes into infinitle loop and odesnt work 
		}else if(n==2){
			cout << "Enter the memory address: ";
			cin >> vaddr;
			try{
				val = mmu.loadVal(vaddr);
				cout << "Value at given address is: " << val << endl;
			}catch(const exception& e){
				cout << "Caught exception: " << e.what() << endl;
				cout << "Try again" << endl;
			}
		}else{
			break;
		}
	}
	return 0;
}