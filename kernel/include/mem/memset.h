/*
 *  memset.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the 
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _mem_memset_h
#define _mem_memset_h
#include <inttypes.h>

/*!
    @function memset
    Set bytes at a source address to a value

    @param dst
    The destination address to write to.

    @param c
    The byte to be written

    @param n
    The amount of times to write byte

    @return
    The destination address, solely to match the declaration in string.h
!*/

void *memset(void *dst, int c, size_t n);
#endif /* _mem_memset_h */