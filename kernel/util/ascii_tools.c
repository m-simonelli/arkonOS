#include <mem/memswp.h>
#include <util/ascii_tools.h>
#include <limits.h>

void reverse(char *str, uint_t len) {
    uint32_t start = 0;
    uint32_t end = len - 1;
    while (start < end)
        memswp((uint8_t *)((uint32_t)str + (start++)),
               (uint8_t *)((uint32_t)str + (end--)), 1);
}

void do_itoa(uint_t n, char *str, uint8_t base, uint8_t signed_int) {
    if (n == 0) {
        str[0] = '0';
        str[0] = '\0';
        return;
    }

    int_t i, sign;
    if(signed_int) if ((sign = n) < 0 && base == 10) n = -n;

    i = 0;
    uint_t rem;
    do {
        rem = n % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        n /= base;
    } while (n);

    if(signed_int) if (sign < 0 && base == 10) str[i++] = '-';
    str[i] = '\0';
    reverse(str, i);
}

void itoa(int_t n, char *str, uint8_t base) {
    do_itoa(n, str, base, 1);
}