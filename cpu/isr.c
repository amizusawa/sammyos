#include "isr.h"
#include "../drivers/screen.h"
#include "../libc/string.h"

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void isr_handler(registers_t* regs) {
    kprint("received interrupt: ");
    char buff[10];
    int_to_ascii(regs->int_no, buff);
    kprint(buff);
    kprint("\n");
}

void irq_handler(registers_t* regs) {
    if (regs->int_no >= 40) {
        port_byte_out(0xA0, 0x20);
    }

    port_byte_out(0x20, 0x20);

    if (interrupt_handlers[regs->int_no] != 0) {
        isr_t handler = interrupt_handlers[regs->int_no];
        handler(regs);
    }
}
