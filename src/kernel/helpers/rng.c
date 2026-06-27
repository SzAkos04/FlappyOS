#include "rng.h"

// A simple LCG random number generator
static uint32_t next_random = 123456789;

void rand_seed(uint32_t seed) { next_random = seed; }

// Returns a pseudo-random integer between min and max (inclusive)
int rand_range(int min, int max) {
    next_random = next_random * 1103515245 + 12345;
    int rand_val = (unsigned int)(next_random / 65536) % 32768;
    return min + (rand_val % (max - min + 1));
}
