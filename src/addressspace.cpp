#include "addressspace.hpp"

kernel::AddressSpace::AddressSpace(MemoryAllocator& malloc)
  : malloc(malloc)
{
	this->pageTables = (PageTableEntry*) 0xFFC00000;
	this->pageDirectory = (PageTableEntry*) 0xFFFFF000;
}

void* kernel::AddressSpace::mmap(void* start, size_t length)
{
	size_t tableIndex = (size_t) start / 4096;
	for(int i = (int) length; i > 0; i -= 4096)
	{
		size_t directoryIndex = tableIndex / 1024;
		if(!pageDirectory[directoryIndex].getPresent())
		{
			physaddr_t newPT = malloc.allocate(4096);
			pageDirectory[directoryIndex] = newPT;
			pageDirectory[directoryIndex].setPresent(true);
			pageDirectory[directoryIndex].setUsermode(false);
			pageDirectory[directoryIndex].setRw(true);
		}
		if(!pageTables[tableIndex].getPresent())
		{
			physaddr_t page = malloc.allocate(4096);
			pageTables[tableIndex] = page;
			pageTables[tableIndex].setUsermode(false);
			pageTables[tableIndex].setRw(true);
			pageTables[tableIndex].setPresent(true);
		}
		tableIndex++;
	}
	return start;
}

void kernel::AddressSpace::munmap(void* start, size_t length)
{

}

physaddr_t kernel::AddressSpace::getPhysicalAddress(void* virtualAddress)
    const
{
	size_t index = (size_t) virtualAddress / 4096;
	PageTableEntry pte = pageTables[index];
	if(pte.getPresent())
		return pte.getPhysicalAddress();
	else
		return 0;
}
