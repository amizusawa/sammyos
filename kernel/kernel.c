#include "kernel.h"
#include "multiboot.h"
#include "frame_alloc.h"
#include "../drivers/screen.h"
#include "../cpu/descriptor_tables.h"
#include "../drivers/timer.h"
#include "../drivers/keyboard.h"
#include "thread.h"

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
    asm volatile("sti");
    init_timer(50);
    init_keyboard();
    init_thread();

}

void test_thread(char* str) {
    for (int i = 0; i < 5; i++) {
        char chr[2] = {str[i], '\0'};
        kprint(chr);
        kprint("\n");
    }
}
