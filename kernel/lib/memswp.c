/*
 *  memswp.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <memswp.h>
void memswp(const char *m1, const char *m2, size_t size) {
    byte_t tmp;
    for (count_t i = 0; i < size; i++) {
        tmp = *((char *)((size_t)m1 + i));
        *((char *)((size_t)m1 + i)) = *((char *)((size_t)m2 + i));
        *((char *)((size_t)m2 + i)) = tmp;
    }
}