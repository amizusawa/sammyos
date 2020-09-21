#include "keyboard.h" 
#include "ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"

#define SC_MAX 57
#define L_SHIFT 0x2a
#define R_SHIFT 0x36
#define L_SHIFT_RELEASED 0xaa
#define R_SHIFT_RELEASED 0xb6

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
                                '=', '?', '?', 'q', 'w', 
                                'e', 'r', 't', 'y', 'u', 'i', 'o', 
                                'p', '[', ']', '\n', '?', 'a', 
                                's', 'd', 'f', 'g', 'h', 'j', 'k', 
                                'l', ';', '\'', '`', '?', '\\', 
                                'z', 'x', 'c', 'v', 'b', 'n', 'm', 
                                ',', '.', '/', '?', '?', 
                                '?', ' '};


enum key shift_key = released;

void shift_key_convert(char* key) {
    switch(*key) {
        // a-z
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x69:
        case 0x6a:
        case 0x6b:
        case 0x6c:
        case 0x6d:
        case 0x6e:
        case 0x6f:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x78:
        case 0x79:
        case 0x7a:
            *key -= 0x20;
            break;
    }
}

static void keyboard_callback(registers_t regs) {
    UNUSED(regs);

    uint8_t scancode = port_byte_in(0x60);

    
    if (scancode == L_SHIFT || scancode == R_SHIFT) {
        shift_key = pressed;
        return;
    }
    else if (scancode == L_SHIFT_RELEASED || scancode == R_SHIFT_RELEASED) {
        shift_key = released;
        return;
    }
    else if (scancode >> 7) return;
    
    char letter = scancode_ascii[(int) scancode];
    if (shift_key == pressed) shift_key_convert(&letter); 
    char str[2] = {letter, '\0'};
    kprint(str);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, &keyboard_callback);
}
