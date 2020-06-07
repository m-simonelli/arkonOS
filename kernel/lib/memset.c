/*
 *  memset.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <memset.h>
void *memset(void *restrict dst, int c, size_t n) {
    for (count_t i = 0; i < n; i++) ((uint8_t *)dst)[i] = c;
    return dst;
}