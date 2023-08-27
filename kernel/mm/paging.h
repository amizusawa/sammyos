#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>
#include <cpu/multiboot.h>

#define PAGE_FRAME_MASK 0xFFFFF000
#define PAGE_PRESENT 1

typedef uint32_t directory_entry_t;
typedef uint32_t page_t;

void init_page_fault_handler();

#endif
