/*
 *  memcpy.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <mem/memcpy.h>
void *memcpy(void *to, const void *from, size_t len) {
    for (count_t i = 0; i < len; i++)
        *((char *)((size_t)to + i)) = *((char *)((size_t)from + i));
    return to;
}