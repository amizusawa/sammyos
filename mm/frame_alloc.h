#ifndef FRAME_ALLOC_H
#define FRAME_ALLOC_H

#include <stdint.h>
#include <stddef.h>
#include "../kernel/multiboot.h"

#define MMAP_GET_NUM 0
#define MMAP_GET_ADDR 1
#define PAGE_SIZE 4096
#define MAX_NUM_PAGES 1024
#define PTR_SIZE 4

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

#endif
