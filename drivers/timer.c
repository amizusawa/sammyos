#include "timer.h"
#include "screen.h"
#include "../cpu/isr.h"
#include "ports.h"
#include "../kernel/utils.h"

u32 tick = 0;

static void timer_callback(registers_t regs) {
    tick++;
}

void init_timer(u32 freq) {
    register_interrupt_handler(IRQ0, &timer_callback);

    u32 divisor = 1131980 / freq;
    port_byte_out(0x43, 0x36);

    u8 low = (u8) (divisor & 0xff);
    u8 high = (u8) ((divisor >> 8) & 0xff);

    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}

