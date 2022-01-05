#ifndef VADDR_H
#define VADDR_h

#include <stdint.h>

#define LOADER_PHYS_BASE 0xC0000000

#define PALLOC_H
#define MMAP_GET_NUM 0
#define MMAP_GET_ADDR 1
#define MAX_NUM_PAGES 1024

#define PTR_SIZE 4
#define BITMASK(SHIFT, CNT) (((1ul << (CNT)) - 1) << (SHIFT))

#define PGSHIFT 0
#define PGBITS  12
#define PAGE_SIZE  (1 << PGBITS)         
#define PGMASK  BITMASK(PGSHIFT, PGBITS)

/* Offset within a page. */
static inline unsigned pg_ofs (const void *va) {
  return (uintptr_t) va & PGMASK;
}

/* Virtual page number. */
static inline uintptr_t pg_no (const void *va) {
  return (uintptr_t) va >> PGBITS;
}

/* Round up to nearest page boundary. */
static inline void *pg_round_up (const void *va) {
  return (void *) (((uintptr_t) va + PAGE_SIZE - 1) & ~PGMASK);
}

/* Round down to nearest page boundary. */
static inline void *pg_round_down (const void *va) {
  return (void *) ((uintptr_t) va & ~PGMASK);
}

static inline void* ptov(uintptr_t paddr) {
    return (void*) paddr + LOADER_PHYS_BASE;
}

static inline uintptr_t vtop(const void* vaddr) {
    return  (uintptr_t) vaddr - (uintptr_t) LOADER_PHYS_BASE;
}

#endif
