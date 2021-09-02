#include "palloc.h"
#include <mem.h>
#include <function.h>

multiboot_info_t* verified_mboot_hdr;
uint32_t mboot_reserved_start;
uint32_t mboot_reserved_end;
uint32_t next_free_frame;
extern uint8_t _end_kernel;
struct page_frame pages[MAX_NUM_PAGES];

int round_nearest_power_2(uint32_t num, uint32_t multiple);

void init_palloc(multiboot_info_t* mboot_hdr) {
    UNUSED(mboot_hdr);

    uint8_t* page_addr = &_end_kernel;
    page_addr = (uint8_t*) round_nearest_power_2(
                                (uint32_t) page_addr, 
                                PAGE_SIZE);

    for (int i = 0; i < MAX_NUM_PAGES; i++) {
        struct page_frame page;
        page.page_num = i;
        page.page_addr = (void*) page_addr;
        //page.status = PAGE_FREE;

        page_addr += PAGE_SIZE;

        pages[i] = page;
    }

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

void* palloc_get_page(enum palloc_flags flags) {

}

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

uint32_t frame_free(struct page_frame* page) {
    /*
    if (page->status == PAGE_FREE) return -1;

    memset(page->page_addr, 0, PAGE_SIZE);

    page->status = PAGE_FREE;
    */
    return 0;
}
