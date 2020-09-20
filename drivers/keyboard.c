#include "keyboard.h" 
#include "ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"

#define SC_MAX 57

const char* scancode_chars[] = {"ERROR", "Esc", "1", "2", "3", "4", 
                                "5", "6", "7", "8", "9", "0", "-", 
                                "=", "Backspace", "Tab", "Q", "W", 
                                "E", "R", "T", "Y", "U", "I", "O", 
                                "P", "[", "]", "Enter", "Lctrl", "A", 
                                "S", "D", "F", "G", "H", "J", "K", 
                                "L", ";", "'", "`", "Lshift", "\\", 
                                "Z", "X", "C", "V", "B", "N", "M", 
                                ",", ".", "/", "Rshift", "Keypad *", 
                                "Lalt", "Spacebar"};

const char scancode_ascii[] =  {'?', '?', '1', '2', '3', '4', 
                                '5', '6', '7', '8', '9', '0', '-', 
                                '=', '?', '?', 'Q', 'W', 
                                'E', 'R', 'T', 'Y', 'U', 'I', 'O', 
                                'P', '[', ']', '?', '?', 'A', 
                                'S', 'D', 'F', 'G', 'H', 'J', 'K', 
                                'L', ';', '\'', '`', '?', '\\', 
                                'Z', 'X', 'C', 'V', 'B', 'N', 'M', 
                                ',', '.', '/', '?', '?', 
                                '?', ' '};


static void keyboard_callback(registers_t regs) {
    UNUSED(regs);

    uint8_t scancode = port_byte_in(0x60);

    if (scancode > SC_MAX) return;
    
    char letter = scancode_ascii[(int) scancode];
    char str[2] = {letter, '\0'};
    kprint(str);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, &keyboard_callback);
}
