#include "pmm.h"
#include <mem.h>
#include <function.h>
#include <math.h>
#include <bitmap.h>
#include <lock.h>
#include <panic.h>

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

    uint32_t* free_start = ptov(3 * 1024 * 1024);

    init_pool(&kernel_pool, free_start, MAX_NUM_PAGES, "kernel page");
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

void* pmm_get_multiple(enum palloc_flags flags, size_t page_cnt) {

    if (page_cnt == 0) return NULL;

    // TODO: when user pool is added check the flags for user pool
    struct pool* pool = &kernel_pool;
    void* pages;
    size_t page_idx;

    lock_acquire(&pool->lock);
    page_idx = bitmap_scan_and_flip(pool->used_map, 0, page_cnt, false);
    lock_release(&pool->lock);

    if (page_idx != BITMAP_ERROR) {
        pages = pool->base + PAGE_SIZE * page_idx;
    }
    else {
        pages = NULL;
    }

    if (pages != NULL) {
        // TODO: add flags like PMM_ZERO to zero out the pages and apply them here
    }
    else {
        // TODO: if PMM_ASSERT is set then panic
    }

    return pages;
}

void* pmm_get_page(enum palloc_flags flags) {
    return pmm_get_multiple(flags, 1);
}

void pmm_free_multiple(void* pages, size_t page_cnt) {
    struct pool* pool = &kernel_pool;
    
    size_t page_idx = pg_no(pages) - pg_no(pool->base);

    // Make sure the pages we're freeing were allocated
    ASSERT(bitmap_all(pool->used_map, page_idx, page_cnt));
    bitmap_set_multiple(pool->used_map, page_idx, page_cnt, false);
}

void pmm_free(void* page) {
    pmm_free_multiple(page, 1);
}
