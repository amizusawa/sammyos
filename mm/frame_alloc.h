#ifndef FRAME_ALLOC_H
#define FRAME_ALLOC_H

#include <stdint.h>
#include "../kernel/multiboot.h"

#define MMAP_GET_NUM 0
#define MMAP_GET_ADDR 1
#define PAGE_SIZE 4096

void init_frame_alloc(multiboot_info_t* mboot_hdr);
uint32_t mmap_read(uint32_t request, uint8_t mode);
uint32_t frame_alloc();

#endif
