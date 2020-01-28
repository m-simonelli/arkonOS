#include <libc/stdarg.h>
#include <inttypes.h>
#include <mem/memset.h>
#include <mem/strlen.h>
#include <util/ascii_tools.h>
#include <mem/string.h>

void vsprintf(char *str, void (*putchar)(char), const char *fmt, va_list ap);