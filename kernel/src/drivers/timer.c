#include <timer.h>
#include <vga.h>
#include <ports.h>
#include <string.h>
#include <arch/interrupt.h>
#include <thread.h>

uint32_t tick = 0;

static void timer_callback(registers_t* regs) {
    UNUSED(regs);

    tick++;
    thread_tick();
}

void init_timer(uint32_t freq) {
    register_interrupt_handler(IRQ0, timer_callback);

    uint32_t divisor = 1131980 / freq;
    port_byte_out(0x43, 0x36);

    uint8_t low = (uint8_t) (divisor & 0xff);
    uint8_t high = (uint8_t) ((divisor >> 8) & 0xff);

    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}

