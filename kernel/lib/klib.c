/*
 *  ascii_tools.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <limits.h>
#include <memswp.h>
#include <klib.h>

void reverse(char *str, uint_t len) {
    size_t start = 0;
    size_t end = len - 1;
    while (start < end)
        memswp((char *)((size_t)str + (start++)),
               (char *)((size_t)str + (end--)), 1);
}

void do_itoa(uint_t n, char *str, uint8_t base, uint8_t signed_int) {
    if (n == 0) {
        str[0] = '0';
        str[0] = '\0';
        return;
    }

    int_t i, sign;
    if (signed_int)
        if ((sign = n) < 0 && base == 10) n = -n;

    i = 0;
    uint_t rem;
    do {
        rem = n % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        n /= base;
    } while (n);

    if (signed_int)
        if (sign < 0 && base == 10) str[i++] = '-';
    str[i] = '\0';
    reverse(str, i);
}

void itoa(int_t n, char *str, uint8_t base) { do_itoa(n, str, base, 1); }