#include "../drivers/screen.h"

void main() {

    char* c = "A\n";
    for (int i = 0; i < 25; i++) {
        kprint(c);
        c[0] = c[0]+1;
    }

    char* string = "helloooooooooooooooooooooo this line should scroll\n";
    kprint_at(string, 24, 78);

}
