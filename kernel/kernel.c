#include "kernel.h"
#include "multiboot.h"
#include "../drivers/screen.h"
#include "../cpu/descriptor_tables.h"
#include "../drivers/timer.h"
#include "../drivers/keyboard.h"


void kernel_main(uint32_t mboot_magic, void* mboot_header) {

    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        kprint("ERROR: Kernel loaded by non-compliant bootloader\n");
        return;
    }

    init_descriptor_tables();
    kprint("Descriptor tables initialized.\n");

    asm volatile("sti");
    init_timer(50);
    kprint("Timer initialized.\n");

    init_keyboard();
    kprint("Keyboard initialized.\n");

}
