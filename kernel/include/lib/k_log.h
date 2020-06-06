/*
 *  k_log.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _k_log_h
#define _k_log_h

#include <inttypes.h>

extern void (*log_putchar)(char);

#define k_log(msg, ...) dbg_log(__func__, __LINE__, msg, ##__VA_ARGS__);
#define k_printf(msg, ...) dbg_log(NULL, -1, msg, ##__VA_ARGS__);
void dbg_log(const char *func, ssize_t line, char *msg, ...);
#define kprintf k_printf

#endif /* _k_log_h */