#include "bitmap.h"
#include <math.h>
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

// Returns number of elements needed for bit_cnt bits
static inline size_t elem_cnt(size_t bit_cnt) {
    return DIV_ROUND_UP(bit_cnt, ELEM_BITS);
}

// Returns number of bytes needed for bit_cnt bits
static inline size_t byte_cnt(size_t bit_cnt) {
    return sizeof(elem_type) * elem_cnt(bit_cnt);
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

size_t bitmap_buf_size(size_t bit_cnt) {
    return sizeof(struct bitmap) + byte_cnt(bit_cnt);
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

// Get the value at idx
bool bitmap_get(const struct bitmap* b, size_t idx) {
    ASSERT(b != NULL);
    ASSERT(idx < b->bit_count);

    return (b->bits[elem_idx(idx)] & bit_mask(idx)) != 0;
}

// Returns true if any bits in b between start and start + cnt, 
// exclusive, are set to value, and false otherwise
bool bitmap_contains(const struct bitmap* b, size_t start, size_t cnt, bool value) {
    ASSERT(b != NULL);
    ASSERT(start <= b->bit_count);
    ASSERT(start + cnt <= b->bit_count);

    size_t i;
    for (i = 0; i < cnt; i++) {
        if (bitmap_get(b, start + i) == value) {
            return true;
        }
    }
    return false;
}

// Finds and returns the starting index of the first group of 
// cnt consecutive bits in b after start that are all set to value. 
// Returns BITMAP_ERROR if none exists
size_t bitmap_scan(const struct bitmap* b, size_t start, size_t cnt, bool value) {
    ASSERT(b != NULL);
    ASSERT(start <= b->bit_count);

    if (cnt <= b->bit_count) {
        size_t last = b->bit_count - cnt;
        size_t i;
        for (i = start; i <= last; i++) {
            if (!bitmap_contains(b, i, cnt, !value)) {
                return i;
            }
        }
    }

    return BITMAP_ERROR;
}

// Finds the first group of cnt consecutive bits in b after start 
// that are all set to value and flips them to !value and returns 
// the first index
size_t bitmap_scan_and_flip(struct bitmap* b, size_t start, size_t cnt, bool value) {
    size_t idx = bitmap_scan(b, start, cnt, value);
    if (idx != BITMAP_ERROR) {
        bitmap_set_multiple(b, idx, cnt, !value);
    }

    return idx;
}

// Returns true if every bit in b between start and start + cnt is 
// set to true and false otherwise
bool bitmap_all(const struct bitmap* b, size_t start, size_t cnt) {
    return !bitmap_contains(b, start, cnt, false);
}
