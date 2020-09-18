#include "kernel.h"
#include "../drivers/screen.h"
#include "../cpu/descriptor_tables.h"
#include "../drivers/timer.h"
#include "../drivers/keyboard.h"

void kernel_main() {

    init_descriptor_tables();
    kprint("Descriptor tables initialized.\n");

    asm volatile("sti");
    init_timer(50);
    kprint("Timer initialized.\n");

    init_keyboard();
    kprint("Keyboard initialized.\n");

}
