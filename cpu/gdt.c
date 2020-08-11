#include "gdt.h"
#include "types.h"

extern void gdt_flush(u32);
static void gdt_set_gate(s32, u32, u32, u8, u8);
static void init_gdt();

gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

void init_descriptor_tables() {
    init_gdt();
}

static void init_gdt() {
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (u32) &gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xffffffff, 0x9a, 0xcf);
    gdt_set_gate(2, 0, 0xffffffff, 0x92, 0xcf);
    gdt_set_gate(3, 0, 0xffffffff, 0xfa, 0xcf);
    gdt_set_gate(4, 0, 0xffffffff, 0xf2, 0xcf);

    gdt_flush((u32) &gdt_ptr);
}

static void gdt_set_gate(s32 num, u32 base, u32 limit, u8 access, u8 gran) {
    gdt_entries[num].base_low = (base & 0xffff);
    gdt_entries[num].base_middle = (base >> 16) & 0xff;
    gdt_entries[num].base_low = (base >> 24) & 0xff;

    gdt_entries[num].limit_low = (limit & 0xffff);
    gdt_entries[num].granularity = (limit >> 16) & 0x0f;

    gdt_entries[num].granularity |= gran & 0xf0;
    gdt_entries[num].access = access;

}
