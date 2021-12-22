#include "paging.h"
#include <cpu/interrupt.h>
#include <stdbool.h>
#include <function.h>

#define DIRECTORY_INDEX(x) ((x) >> 22)
#define TABLE_INDEX(x) (((x) >> 12) & 0x3FF)

void page_fault_handler(registers_t* regs);

void init_page_fault_handler() {
    register_interrupt_handler(IRQ14, page_fault_handler);

}

page_t* paging_get_page(uintptr_t virt, bool create, uint32_t flags) {
    // TODO: Create new page directory if needed
    UNUSED(create);
    UNUSED(flags);

    uint32_t dir_index = DIRECTORY_INDEX(virt);
    uint32_t table_index = TABLE_INDEX(virt);

    directory_entry_t* dir = (directory_entry_t*) 0xFFFFF000;
    page_t* table = (page_t*) (0xFFC00000 + (dir_index << 12));

    if (dir[dir_index] & PAGE_PRESENT) {
        return &table[table_index];
    }

    return NULL;
}

void page_fault_handler(registers_t* regs) {
    uint32_t err = regs->err_code;
    uintptr_t cr2 = 0;
    asm volatile("mov %%cr2, %0\n" : "=r"(cr2));
    
    // TODO: Do permission checks?
    
    page_t* page = paging_get_page(cr2 & PAGE_FRAME_MASK, false, 0);

    // TODO: error checks
}
