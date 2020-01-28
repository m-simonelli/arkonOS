#ifndef _libc_printf_h
#define _libc_printf_h

#include <inttypes.h>
#include <libc/stdarg.h>
#include <mem/memset.h>
#include <mem/string.h>
#include <mem/strlen.h>
#include <util/ascii_tools.h>

void vsprintf(char *str, void (*putchar)(char), const char *fmt, va_list ap);

#endif /* _libc_printf_h */