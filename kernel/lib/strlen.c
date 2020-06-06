/*
 *  strlen.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <strlen.h>

size_t strlen(const char *s) {
    size_t i;
    for (i = 0; s[i] != 0; ++i)
        ;
    return i;
}