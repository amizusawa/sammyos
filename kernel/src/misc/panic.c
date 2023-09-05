#include "panic.h"
#include <screen.h>

// Print kernel panic and halt cpu
void kpanic(char* reason) {
    screen_clear();
    kprint("KERNEL PANIC: ");
    kprint(reason);

    asm __volatile__("hlt");

    //TODO: eventually print backtrace info
    
}

