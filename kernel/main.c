#include <stdint.h>
#include "multiboot.h"
#include "thread.h"
#include <mm/pmm.h>
#include <drivers/screen.h>
#include <cpu/descriptor_tables.h>
#include <cpu/interrupt.h>
#include <drivers/timer.h>
#include <drivers/keyboard.h>
#include <mm/paging.h>

//uint32_t page_dir[1024] __attribute__((aligned(4096)));


void test_func1() {
    uint32_t i = 0;
    for (;;) {
        if (i % 1 << 15 == 0) kprint("Func 1");
        i++;
    }
}

void test_func2() {
    uint32_t i = 0;
    for (;;) {
        if (i % 1 << 15 == 0) kprint("Func 2");
        i++;
    }
}

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
    
    
    thread_create(test_func1);
    thread_create(test_func2);
    
    thread_start();

}
