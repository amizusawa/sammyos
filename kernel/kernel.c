#include "kernel.h"
#include "multiboot.h"
#include "frame_alloc.h"
#include "../drivers/screen.h"
#include "../cpu/descriptor_tables.h"
#include "../drivers/timer.h"
#include "../drivers/keyboard.h"

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

    uint32_t new_frame = frame_alloc();
    uint32_t new_frame_addr = mmap_read(new_frame, MMAP_GET_ADDR);
    char* str = (char*) new_frame_addr;
    for (int i = 0; i < PAGE_SIZE; i++) {
        str[i] = 'a';
    }

    init_descriptor_tables();
    kprint("Descriptor tables initialized.\n");

    asm volatile("sti");
    init_timer(50);
    kprint("Timer initialized.\n");

    init_keyboard();
    kprint("Keyboard initialized.\n");

}
