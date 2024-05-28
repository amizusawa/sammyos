#ifndef ATA_H
#define ATA_H

#include <stdint.h>

void init_ata();
uint8_t ata_detect_devices();

#endif
