#include "../drivers/screen.h"
#include "utils.h"
#include "../cpu/descriptor_tables.h"
#include "../drivers/timer.h"

void kernel_main() {

    init_descriptor_tables();

    asm volatile("sti");
    init_timer(50);

}
