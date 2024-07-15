
#include <ata.h>
#include <stdint.h>
#include <panic.h>
#include <arch/interrupt.h>
#include <ports.h>
#include <vga.h>
#include <function.h>

// The primary and secondary ATA buses
#define CONTROLLER_COUNT 2

// ATA register offsets from IO base
#define REG_DREG(CHANNEL)    ((CHANNEL)->reg_base + 0)  /* Data. */
#define REG_ERROR(CHANNEL)   ((CHANNEL)->reg_base + 1)  /* Error. */
#define REG_NSECT(CHANNEL)   ((CHANNEL)->reg_base + 2)  /* Sector Count. */
#define REG_LBAL(CHANNEL)    ((CHANNEL)->reg_base + 3)  /* LBA 0:7. */
#define REG_LBAM(CHANNEL)    ((CHANNEL)->reg_base + 4)  /* LBA 15:8. */
#define REG_LBAH(CHANNEL)    ((CHANNEL)->reg_base + 5)  /* LBA 23:16. */
#define REG_DEVICE(CHANNEL)  ((CHANNEL)->reg_base + 6)  /* Device/LBA 27:24. */
#define REG_STATUS(CHANNEL)  ((CHANNEL)->reg_base + 7)  /* Status (r/o). */
#define REG_COMMAND(CHANNEL) REG_STATUS(CHANNEL)        /* Command (w/o). */

// ATA commands
typedef enum ata_command {
    IDENTIFY_DEVICE =    0xEC,
    READ_SECTOR_RETRY =  0x20,
    WRITE_SECTOR_RETRY = 0x30,
} ata_command;

struct ata_device {
    uint8_t dev_no;
};

struct controller {
    uintptr_t reg_base;
    uint32_t irq_no;
    struct ata_device devices[2];
};

struct controller controllers[CONTROLLER_COUNT];

static void ata_interrupt_handler(registers_t* regs);
void ata_issue_pio_command(struct controller* c, ata_command command);

void init_ata() {
    for (uint8_t i = 0; i < CONTROLLER_COUNT; i++) {
        struct controller* c = &controllers[i];

        switch (i) {
            case 0:
                c->reg_base = 0x1F0;
                c->irq_no = IRQ14;
                break;
            case 1:
                c->reg_base = 0x170;
                c->irq_no = IRQ15;
                break;
            default:
                NOT_REACHED();
                break;
        }

        register_interrupt_handler(c->irq_no, ata_interrupt_handler);
    }
}

void ata_issue_pio_command(struct controller* c, ata_command command) {
    port_byte_out(REG_COMMAND(c), command);
}

uint8_t ata_detect_devices() {
    struct controller* c = &controllers[0];
    ata_issue_pio_command(c, IDENTIFY_DEVICE);
    return 0;
}

static void ata_interrupt_handler(registers_t* regs) {
    kprint("In ata interrupt handler\n");
}
