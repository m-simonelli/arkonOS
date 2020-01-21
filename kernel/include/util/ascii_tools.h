#ifndef _util_ascii_tools_h
#define _util_ascii_tools_h

#include <inttypes.h>

/*!
    @function itoa
    Convert a number to a string containing those numbers as text

    @param n
    The number to be converted to a string

    @param str
    The address of a string to write the result to

    @param base
    What base the string should have the number as, i.e. hex, decimal, octal.
    This is a number representing the base, hex is 16, decimal is 10, etc.
!*/
void itoa(int_t n, char *str, uint8_t base);
#endif /* _util_ascii_tools_h */