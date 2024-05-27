#include "panic.h"
#include <vga.h>

// Print kernel panic and halt cpu
void kpanic(char* reason) {
    vga_screen_clear();
    kprint("KERNEL PANIC: ");
    kprint(reason);

    asm __volatile__("hlt");

    //TODO: eventually print backtrace info
    
}

