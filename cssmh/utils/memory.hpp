#ifndef MEMORY_HPP
#define MEMORY_HPP

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <unistd.h>
#include <sys/mman.h>

int GetMemoryProtection(void *addr);
bool ProtectMemory(void *addr, size_t size, int prot);

#endif
