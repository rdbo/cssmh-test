#include "memory.hpp"
#include <iostream>

bool ProtectMemory(void *addr, size_t size, int prot)
{
	const static uintptr_t pagesize = (uintptr_t)sysconf(_SC_PAGE_SIZE);
	void *aligned_addr = (void *)((uintptr_t)addr & -pagesize);
	size_t diff = (size_t)((uintptr_t)addr - (uintptr_t)aligned_addr);
	size += diff;
	return !mprotect(aligned_addr, size, prot);
}
