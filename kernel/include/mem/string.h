#ifndef _mem_string_h
#define _mem_string_h

#include <inttypes.h>
#include <mem/memset.h>

/*!
    @function strcpy
    Copy null terminated string from a source to a destination

    @param dst
    A pointer to the destination address where the string should
    be copied to

    @param src
    A pointer to the source address where the string should be
    copied from

    @return
    A pointer to the address where the string was copied to.
    This should match the `dst` parameter.
!*/
char *strcpy(char *dst, char const *src);

/*!
    @function bzero
    A function-like macro that will set n bytes starting at address to zero

    @param d
    The address to be zeroed

    @param n
    The amount of bytes to be zeroed

    @return
    The provided parameter `d`
!*/
#define bzero(d, n) memset((void *)d, 0, n)

#endif /* _mem_string_h */