/*
 *  klib.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _lib_klib_h
#define _lib_klib_h

#include <inttypes.h>

/*!
    @function itoa
    Convert a number to a string containing those numbers as text.

    @param n
    The number to be converted to a string

    @param str
    The address of a string to write the result to

    @param base
    What base the string should have the number as, i.e. hex, decimal, octal.
    This is a number representing the base, hex is 16, decimal is 10, etc.

    @discussion
    This function will always treat the number as a signed integer. This
    function wraps `do_itoa` setting the `signed_int` parameter of it to 1
    unconditionally
!*/
void itoa(int_t n, char *restrict str, uint8_t base);

/*!
    @function do_itoa
    Convert a number to a string containing those numbers as text.

    @param n
    The number to be converted to a string

    @param str
    The address ot a string to write the result to

    @param base
    What base the string should have the number as, i.e. hex, decimal, octal.
    This is a number representing the base, hex is 16, decimal is 10, etc.

    @param signed_int
    If greater than 0, treat `n` as a signed integer, else treat it unsigned
!*/
void do_itoa(uint_t n, char *restrict str, uint8_t base, uint8_t signed_int);

int atoi(const char *restrict s);
#endif /* _lib_klib_h */