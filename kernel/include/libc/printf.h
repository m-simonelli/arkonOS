#ifndef _libc_printf_h
#define _libc_printf_h

#include <inttypes.h>
#include <libc/stdarg.h>
#include <mem/memset.h>
#include <mem/string.h>
#include <mem/strlen.h>
#include <util/ascii_tools.h>

/*!
    @function vsprintf
    Parse a string with format tags, and print each character using a given
    character printing function

    @param str
    Unused. Reserved for future use

    @param putchar
    A pointer to a function that takes 1 argument, a char, that is used to
    print each character

    @param fmt
    A null terminated string, optionally containing format tags to be parsed.
    Format tags are in the format %<flags><width><length><specifier>.
    Described below are possible values for all fields of the format tag:
        flags:
            '-' Left justify the current field
            '+' Forces numbers to have a '+' or '-' sign preceding them,
                even for positive numbers (default is only for negative
                numbers)
            ' ' Insert a blank space before the value
            '#' Will insert a '0', '0x', or '0X' for specifiers 'o', 'x',
                and 'X' respectively.
            '0' Left pad the number with 0's instead of spaces if width is
                specified
        width:
            '[0-9]' Minimum number of characters to be printed
            '*' The minimum number of characters to be printed is given as a
                seperate argument
        length:
            All the following arguments are only valid when the specifier is
            one of i, d, o, u, p, x or X
            'h' The number is interpreted as a short
            'hh' The number is interpreted as a char
            'l' The number is interpreted as a long
            'll' The number is interpreted as a long long
        specifier:
            'c' The value is a 1 byte value to be printed as a single ascii
                character
            'd' or 'i' The value is a signed decimal int
            'u' The value is an unsigned decimal int
            'o' The value is to be printed as an octal number
            'x' The value is to be printed as a hex value
            'X' The value is to be printed as a hex value in uppercase
            'p' The value is an address
            '%' Print the '%' character

    @param ap
    A va_list containing values to match each format tag
!*/

void vsprintf(char *str, void (*putchar)(char), const char *fmt, va_list ap);

#endif /* _libc_printf_h */