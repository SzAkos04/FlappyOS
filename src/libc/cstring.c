#include "cstring.h"

size_t strlen(const char *str) {
    int l = 0;
    while (*str++) {
        l++;
    }
    return l;
}

void memset(void *dst, uint8_t value, size_t n) {
    uint8_t *d = dst;
    while (n-- > 0) {
        *d++ = value;
    }
}

// FIX: memcpy was missing entirely. Added to support swap_buffers row copy in
// screen.c and for general use throughout the kernel.
void *memcpy(void *dst, const void *src, size_t n) {
    uint8_t *d = dst;
    const uint8_t *s = src;
    while (n-- > 0) {
        *d++ = *s++;
    }
    return dst;
}

char *utoa(uint32_t value, char *buffer) {
    char temp[10];
    int i = 0;

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    while (value > 0) {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    }

    int j = 0;
    while (i > 0) {
        buffer[j++] = temp[--i];
    }

    buffer[j] = '\0';
    return buffer;
}
