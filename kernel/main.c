#include <stdint.h>
#include "multiboot.h"
#include "../mm/frame_alloc.h"
#include "../drivers/screen.h"
#include "../cpu/descriptor_tables.h"
#include "../drivers/timer.h"
#include "../drivers/keyboard.h"

uint32_t page_dir[1024] __attribute__((aligned(4096)));

static void init_paging();

void kernel_main(uint32_t mboot_magic, void* mboot_header) {

    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        kprint("ERROR: Kernel loaded by non-compliant bootloader\n");
        return;
    }

    multiboot_info_t* mboot_hdr = (multiboot_info_t *) mboot_header;
    
    if ((mboot_hdr->flags & (1<<6)) == 0) {
        kprint("ERROR: No multiboot memory map was provided\n");
        return;
    }

    init_frame_alloc(mboot_hdr);

    init_descriptor_tables();
    kprint("Descriptor tables initialized.\n");

    asm volatile("sti");
    init_timer(50);
    kprint("Timer initialized.\n");

    init_keyboard();
    kprint("Keyboard initialized.\n");

    init_paging();
    kprint("Test.\n");
}

static void init_paging() {

    for (int i = 0; i < 1024; i++) {
        page_dir[i] = 0x00000002;
    } 


}
