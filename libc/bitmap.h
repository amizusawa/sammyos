#ifndef _BITMAP_H
#define _BITMAP_H

#include <stdint.h>
#include <stddef.h>

struct bitmap* bitmap_create_in_buf(size_t cnt, void* block);

#endif
