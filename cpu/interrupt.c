#include "interrupt.h"

void intr_enable() {
    __asm__ __volatile__("sti");
}

void intr_disable() {
    __asm__ __volatile__("cli");
}
