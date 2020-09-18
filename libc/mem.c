#include "mem.h"

void* memset(void* ptr, char c, int n) {
    if (n > 0) {

        char* p = ptr;
        do {
            *p = c;
            p++;
            n--;
        } while (n > 0);
    }
    return ptr;
}
