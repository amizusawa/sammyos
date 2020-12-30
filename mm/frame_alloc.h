#ifndef FRAME_ALLOC_H
#define FRAME_ALLOC_H

#include <stdint.h>
#include <stddef.h>
#include "../kernel/multiboot.h"

#define MMAP_GET_NUM 0
#define MMAP_GET_ADDR 1
#define MAX_NUM_PAGES 1024
#define PTR_SIZE 4

#define BITMASK(SHIFT, CNT) (((1ul << (CNT)) - 1) << (SHIFT))

#define PGSHIFT 0
#define PGBITS  12
#define PAGE_SIZE  (1 << PGBITS)         
#define PGMASK  BITMASK(PGSHIFT, PGBITS)   

enum {
    PAGE_FREE,
    PAGE_USED
};

struct page_frame {
    uint32_t page_num;
    void* page_addr;
    uint32_t status;
};

void init_frame_alloc(multiboot_info_t* mboot_hdr);
struct page_frame* frame_alloc();
uint32_t frame_free(struct page_frame* page);

static inline void* page_round_down(const void* va) {
    return (void *) ((uintptr_t) va & ~PGMASK);
}

static inline void* page_round_up(const void* va) {
    return (void *) (((uintptr_t) va + PAGE_SIZE - 1) & ~PGMASK);
}

#endif
