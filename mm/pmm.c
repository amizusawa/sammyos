#include "pmm.h"
#include <mem.h>
#include <function.h>
#include <math.h>
#include <bitmap.h>
#include <kernel/lock.h>
#include <kernel/panic.h>

struct pool {
    struct lock lock;
    struct bitmap* used_map;
    uint8_t* base;
};

uint32_t mboot_reserved_start;
uint32_t mboot_reserved_end;
uint32_t next_free_frame;
extern uint32_t _end_kernel;
static struct pool kernel_pool;

int round_nearest_power_2(uint32_t num, uint32_t multiple);
void init_pool(struct pool* p, void* base, size_t page_cnt, const char* name);

void init_pmm(multiboot_info_t* mboot_hdr) {

    uint32_t* kernel_end = &_end_kernel;
    uint32_t mod_count = mboot_hdr->mods_count;
    multiboot_module_t* mod = (multiboot_module_t*) mboot_hdr->mods_addr;
    for (uint32_t i = 0; i < mod_count; i++) {
        kernel_end = max(mod->mod_end, (uint32_t) kernel_end);
        mod = (multiboot_module_t*) (uintptr_t) mod + sizeof(multiboot_module_t);
    }

    init_pool(&kernel_pool, kernel_end, MAX_NUM_PAGES, "kernel page");
    // TODO: Probably need user pool
}

void init_pool(struct pool* p, void* base, size_t page_cnt, const char* name) {
    UNUSED(name);

    // TODO: Calculate space needed for bitmap and subtract from pool
    // TODO: Init pool lock
    size_t bitmap_pages = DIV_ROUND_UP(bitmap_buf_size(page_cnt), PAGE_SIZE);
    if (bitmap_pages > page_cnt) {
        PANIC("Not enough memory for bitmap");
    }
    page_cnt -= bitmap_pages;

    lock_init(&p->lock);
    p->used_map = bitmap_create_in_buf(page_cnt, base);
    p->base = base + bitmap_pages * PAGE_SIZE;

}

int round_nearest_power_2(uint32_t num, uint32_t multiple) {
    return (num + multiple - 1) & (-multiple);
}

struct page_frame* frame_alloc() {
    /*
    for (int i = 0; i < MAX_NUM_PAGES; i++) {
        struct page_frame* page = &pages[i];
        if (page->status == PAGE_FREE) {
            page->status = PAGE_USED;
            return page;
        }
    }
    */

    return NULL;
}

void* pmm_get_page(enum palloc_flags flags) {

}

/*
uint32_t mmap_read(uint32_t request, uint8_t mode) {
    
    if (request == 0) return 0;
    if (mode != MMAP_GET_NUM && mode != MMAP_GET_ADDR) return 0;

    uintptr_t cur_mmap_addr = (uintptr_t) verified_mboot_hdr->mmap_addr;
    uintptr_t mmap_end_addr = cur_mmap_addr + verified_mboot_hdr->mmap_length;
    uint32_t cur_num = 0;

    while (cur_mmap_addr < mmap_end_addr) {
        multiboot_memory_map_t* current_entry = (multiboot_memory_map_t *) cur_mmap_addr;

        uint64_t i;
        uint64_t current_entry_end = current_entry->addr + current_entry->len;
        for (i = current_entry->addr; i + PAGE_SIZE < current_entry_end; i += PAGE_SIZE) {
            if (mode == MMAP_GET_NUM && request >= i && request <= i + PAGE_SIZE) {
                return cur_num + 1;
            }

            if (current_entry->type == MULTIBOOT_MEMORY_RESERVED) {
                if (mode == MMAP_GET_ADDR && cur_num == request) {
                    request++;
                }
                cur_num++;
                continue;
            }
            else if (mode == MMAP_GET_ADDR && cur_num == request) {
                return i;
            }
            cur_num++;
        }

        cur_mmap_addr += current_entry->size + sizeof(uintptr_t);
    }

    return 0;
}
*/

void pmm_free(struct page_frame* page) {
    /*
    if (page->status == PAGE_FREE) return -1;

    memset(page->page_addr, 0, PAGE_SIZE);

    page->status = PAGE_FREE;
    */
    return 0;
}
