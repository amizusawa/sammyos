#ifndef _BITMAP_H
#define _BITMAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct bitmap* bitmap_create_in_buf(size_t cnt, void* block);

size_t bitmap_size(const struct bitmap* b);
size_t bitmap_buf_size(size_t bit_cnt);

void bitmap_set(struct bitmap* b, size_t idx, bool value);
void bitmap_set_all(struct bitmap* b, bool value);
void bitmap_set_multiple(struct bitmap* b, size_t start, size_t cnt, bool value);

#endif
