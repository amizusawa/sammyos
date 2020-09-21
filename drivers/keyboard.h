#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "../libc/function.h"

enum key {pressed, released};

void init_keyboard();

#endif
