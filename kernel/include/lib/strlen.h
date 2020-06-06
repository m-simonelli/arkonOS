/*
 *  strlen.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _mem_strlen_h
#define _mem_strlen_h

#include <inttypes.h>

/*!
    @function strlen
    Get the length of a null terminated string

    @param s
    A pointer to a null terminated string to get the size of

    @return
    The length of `s`
!*/
size_t strlen(const char *s);

#endif /* _mem_strlen_h */