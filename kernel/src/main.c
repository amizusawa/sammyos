#include <stdint.h>
#include <arch/multiboot.h>
#include <thread.h>
#include <pmm.h>
#include <vga.h>
#include <arch/descriptor_tables.h>
#include <arch/interrupt.h>
#include <timer.h>
#include <keyboard.h>
#include <paging.h>

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
    intr_disable();
    
    init_page_fault_handler();
    init_pmm(mboot_hdr);

    init_descriptor_tables();
    kprint("Descriptor tables initialized.\n");

    init_timer(50);
    kprint("Timer initialized.\n");

    init_keyboard();
    kprint("Keyboard initialized.\n");

    init_thread();

    // Interrupts enabled by thread_start
    thread_start();

}
