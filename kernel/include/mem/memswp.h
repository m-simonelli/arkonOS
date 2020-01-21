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
void memswp(mem_ptr_u8_t m1, mem_ptr_u8_t m2, size_t size);
#endif /* _heap_memswp_h */