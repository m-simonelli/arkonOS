#ifndef _heap_memcpy_h
#define _heap_memcpy_h
#include <inttypes.h>
/*!
    @function memcpy
    Copy bytes from a source address to a destination address

    @param dst
    The destination address to write to.

    @param src
    The address from which to read from.

    @param size
    The amount of bytes to be copied
!*/
void memcpy(mem_ptr_u8_t dst, const mem_ptr_t src, size_t size);
#endif /* _heap_memcpy_h */