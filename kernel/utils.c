#include "utils.h"

void int_to_ascii(int n, char buffer[]) {
    int sign;
    if ((sign = n) < 0) n = -n;

    int i = 0;
    do {
        buffer[i++] = (n % 10) + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) buffer[i++] = '-';
    
    buffer[i--] = '\0';

    int j = 0;
    char temp;
    while (j < i) {
        temp = buffer[j];
        buffer[j] = buffer[i];
        buffer[i] = temp;
        i--;
        j++;
    }
}

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
