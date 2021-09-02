#include "bitmap.h"
#include <round.h>

// Block type
// A collection of blocks make up the bitmap. 
// If bit 0 of a block is bit X in the bitmap, 
// bit 1 of the same block is bit X+1 in the bitmap.
typedef unsigned long block_type;

// Number of bits in a block
#define BLOCK_BITS (sizeof(block_type) * 8)

struct bitmap {
    size_t bit_count;
    block_type* bits;
};

// Return the block index containing bit index
static inline block_idx(size_t bit_idx) {
    return bit_idx / BLOCK_BITS;
}


