#include <mem/memset.h>
void *memset(void *dst, int c, size_t n) {
    for (count_t i = 0; i < n; i++) ((u8 *)dst)[i] = c;
    return dst;
}