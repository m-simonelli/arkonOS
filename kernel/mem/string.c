#include <mem/string.h>

char *strcpy(char *dst, char const *src) {
    count_t i = 0;
    while (src[i]) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = 0;
    return dst;
}