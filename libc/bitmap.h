#ifndef _BITMAP_H
#define _BITMAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define BITMAP_ERROR UINT32_MAX

struct bitmap* bitmap_create_in_buf(size_t cnt, void* block);

size_t bitmap_size(const struct bitmap* b);
size_t bitmap_buf_size(size_t bit_cnt);

void bitmap_set(struct bitmap* b, size_t idx, bool value);
void bitmap_set_all(struct bitmap* b, bool value);
void bitmap_set_multiple(struct bitmap* b, size_t start, size_t cnt, bool value);
size_t bitmap_scan(const struct bitmap* b, size_t start, size_t cnt, bool);
size_t bitmap_scan_and_flip(struct bitmap* b, size_t start, size_t cnt, bool);
bool bitmap_get(const struct bitmap* b, size_t idx);
bool bitmap_all(const struct bitmap* b, size_t start, size_t cnt);

#endif
