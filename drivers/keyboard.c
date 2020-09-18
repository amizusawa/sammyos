#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"

static void keyboard_callback(registers_t regs) {
    UNUSED(regs);

    uint8_t scancode = port_byte_in(0x60);

    kprint("Keypress: ");
    char sc_ascii[255];
    int_to_ascii(scancode, sc_ascii);
    kprint(sc_ascii);
    kprint("\n");
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, &keyboard_callback);
}
