#include "../drivers/screen.h"
#include "utils.h"

void main() {
    char buffer[255];
    int_to_ascii(254, buffer);
    kprint(buffer);
    kprint("\n");

    int_to_ascii(10, buffer);
    kprint(buffer);
    kprint("\n");

    int_to_ascii(-10, buffer);
    kprint(buffer);
    kprint("\n");

    screen_clear();
}
