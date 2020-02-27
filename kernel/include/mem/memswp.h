/*
 *  memswp.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the 
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _heap_memswp_h
#define _heap_memswp_h
#include <inttypes.h>

/*!
    @function memswp
    Swap bytes from 2 addresses

    @param m1
    A memory address where bytes will be swapped with bytes from m2

    @param m2
    A memory address where bytes will be swapped with bytes from m1

    @param size
    The amount of bytes to swap
!*/
void memswp(const char *m1, const char *m2, size_t size);
#endif /* _heap_memswp_h */