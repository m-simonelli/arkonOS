#include <k_log.h>
#include <libc/printf.h>
#include <mem/string.h>
#include <panic.h>
#include <util/ascii_tools.h>

void (*log_putchar)(char) = NULL;

void dbg_log(const char *func, ssize_t line, char *msg, ...) {
    if(func && line > 0){
        char ln[256] = {0};

        if (log_putchar == NULL)
            panic("dbg_log called with log_putchar set to NULL!");

        /* Write the filename and line number */
        /* [filename:linenumber] */
        log_putchar('[');
        for (size_t i = 0; i < strlen(func); i++) log_putchar(func[i]);
        log_putchar(':');
        do_itoa(line, (char *)&ln, 10, 0);
        for (size_t i = 0; i < strlen(ln); i++) log_putchar(ln[i]);
        log_putchar(']');
        log_putchar(' ');
    }

    /* Standard va_list setup */
    va_list(ap);
    va_start(ap, msg);
    /* Call vsprintf with the va_list and the string with format tags */
    vsprintf(NULL, log_putchar, msg, ap);
    va_end(ap);
}