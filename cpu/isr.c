#include "types.h"
#include "isr.h"
#include "../drivers/screen.h"
#include "../kernel/utils.h"

void isr_handler(registers_t regs) {
    kprint("received interrupt: ");
    char buff[10];
    int_to_ascii(regs.int_no, buff);
    kprint(buff);
    kprint("\n");
}
