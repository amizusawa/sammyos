#ifndef VADDR_H
#define VADDR_h

#include <stdint.h>

#define LOADER_PHYS_BASE 0xC0000000

static inline void* ptov(uintptr_t paddr) {
    return (void*) paddr + LOADER_PHYS_BASE;
}

static inline uintptr_t vtop(const void* vaddr) {
    return  (uintptr_t) vaddr - (uintptr_t) LOADER_PHYS_BASE;
}

#endif
