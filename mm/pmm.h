#ifndef PALLOC_H
#define PALLOC_H

#include <stdint.h>
#include <stddef.h>
#include <kernel/multiboot.h>

#define MMAP_GET_NUM 0
#define MMAP_GET_ADDR 1
#define MAX_NUM_PAGES 1024
#define PTR_SIZE 4

#define BITMASK(SHIFT, CNT) (((1ul << (CNT)) - 1) << (SHIFT))

#define PGSHIFT 0
#define PGBITS  12
#define PAGE_SIZE  (1 << PGBITS)         
#define PGMASK  BITMASK(PGSHIFT, PGBITS)   

enum palloc_flags {
    PALLOC_NONE = 0,
    PALLOC_ASSERT = 1, // Panic on failure
};

struct page_frame {
    uint32_t page_num;
    void* page_addr;
    uint32_t status;
};

void init_pmm(multiboot_info_t* mboot_hdr);
void* pmm_get_page(enum palloc_flags);
void pmm_free(struct page_frame* page);

static inline void* page_round_down(const void* va) {
    return (void *) ((uintptr_t) va & ~PGMASK);
}

static inline void* page_round_up(const void* va) {
    return (void *) (((uintptr_t) va + PAGE_SIZE - 1) & ~PGMASK);
}

#endif
