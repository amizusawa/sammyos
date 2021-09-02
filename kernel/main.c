#include <stdint.h>
#include "multiboot.h"
#include "thread.h"
#include <mm/palloc.h>
#include <drivers/screen.h>
#include <cpu/descriptor_tables.h>
#include <cpu/interrupt.h>
#include <drivers/timer.h>
#include <drivers/keyboard.h>

uint32_t page_dir[1024] __attribute__((aligned(4096)));

static void init_paging();
extern void load_page_dir(uint32_t *);
extern void enable_paging();

void test_func1();
void test_func2();

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

    init_palloc(mboot_hdr);

    init_paging();
    kprint("Paging enabled.\n");

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

static void init_paging() {

    for (int i = 0; i < 1024; i++) {
        page_dir[i] = 0x00000002;
    } 

    uint32_t first_page_table[1024] __attribute__((aligned(4096)));
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | 3;
    }
    page_dir[0] = ((unsigned int) first_page_table) | 3;

    load_page_dir(page_dir);
    enable_paging();
}

void test_func1() {
    for (int i = 0;i<10;i++) {
        kprint("a");
    }
}

void test_func2() {
    for (int i = 0;i<10;i++) {
        kprint("b");
    }
}
