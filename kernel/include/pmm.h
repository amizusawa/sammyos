#ifndef PALLOC_H
#define PALLOC_H

#include <stdint.h>
#include <stddef.h>
#include <arch/multiboot.h>
#include <vaddr.h>

enum palloc_flags {
    PMM_ASSERT = 1, // Panic on failure
    PMM_ZERO = 2, // Zero out the pages
};

void init_pmm(multiboot_info_t* mboot_hdr);
void* pmm_get_page(enum palloc_flags);
void* pmm_get_multiple(enum palloc_flags flags, size_t page_cnt);
void pmm_free_multiple(void* page, size_t page_cnt);
void pmm_free(void* page);

#endif
