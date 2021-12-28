#include "bitmap.h"
#include <round.h>
#include <function.h>

// Elem type
// A collection of elements make up the bitmap. 
// If bit 0 of an element is bit X in the bitmap, 
// bit 1 of the same element is bit X+1 in the bitmap.
typedef unsigned long elem_type;

// Number of bits in a block
#define ELEM_BITS (sizeof(elem_type) * 8)

struct bitmap {
    size_t bit_count;
    elem_type* bits;
};

// Return the elem index containing bit index
static inline size_t elem_idx(size_t bit_idx) {
    return bit_idx / ELEM_BITS;
}

// Return an elem_type where only the bit_idx is 
// turned on
static inline elem_type bit_mask(size_t bit_idx) {
    return (elem_type) 1 << (bit_idx % ELEM_BITS);
}

// Return the number of bits in b 
size_t bitmap_size(const struct bitmap* b) {
    return b->bit_count;
}

struct bitmap* bitmap_create_in_buf(size_t cnt, void* block) {
    struct bitmap* b = block;

    b->bit_count = cnt;
    b->bits = (elem_type*) (b+1);
    bitmap_set_all(b, false);
    return b;
}

// Atomically set bit at bit_idx to true
void bitmap_mark(struct bitmap* b, size_t bit_idx) {
    size_t idx = elem_idx(bit_idx);
    elem_type mask = bit_mask(bit_idx);

    asm ("orl %1, %0" : "=m" (b->bits[idx]) : "r" (mask) : "cc");
}

// Atomically set bit at bit_idx to false
void bitmap_reset(struct bitmap* b, size_t bit_idx) {
    size_t idx = elem_idx(bit_idx);
    elem_type mask = bit_mask(bit_idx);

    asm ("andl %1, %0" : "=m" (b->bits[idx]) : "r" (~mask) : "cc");
}

// Atomically flip at bit_idx
void bitmap_flip(struct bitmap* b, size_t bit_idx) {
    size_t idx = elem_idx(bit_idx);
    elem_type mask = bit_mask(bit_idx);

    asm ("xorl %1, %0" : "=m" (b->bits[idx]) : "r" (mask) : "cc");
}

void bitmap_set(struct bitmap* b, size_t idx, bool value) {
    ASSERT(b != NULL);
    ASSERT(idx < b->bit_count);

    if (value) {
        bitmap_mark(b, idx);
    }
    else {
        bitmap_reset(b, idx);
    }
}

void bitmap_set_multiple(struct bitmap* b, size_t start, size_t cnt, bool value) {
    ASSERT(b != NULL);
    ASSERT(start <= b->bit_count);
    ASSERT(start + cnt <= b->bit_count);

    for (size_t i = 0; i < cnt; i++) {
        bitmap_set(b, start + i, value);
    }
}

void bitmap_set_all(struct bitmap* b, bool value) {
    ASSERT(b != NULL);

    bitmap_set_multiple(b, 0, bitmap_size(b), value);
}
