/*
 *  memcpy.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the 
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _heap_memcpy_h
#define _heap_memcpy_h
#include <inttypes.h>
/*!
    @function memcpy
    Copy bytes from a source address to a destination address

    @param to
    The destination address to write to.

    @param from
    The address from which to read from.

    @param len
    The amount of bytes to be copied

    @return
    The destination address, solely to match the declaration in string.h
!*/
void *memcpy(void *to, const void *from, size_t len);
#endif /* _heap_memcpy_h */