#include "../drivers/screen.h"

void main() {
    
    kprint_at("hello", 1, 1);
    kprint_at("this spans multiple lines", 10, 75);
    char* test = "this is a test\n";
    kprint_at(test, 2, 0);
}
