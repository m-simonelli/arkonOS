/*
 *  stdarg.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _libc_stdarg_h
#define _libc_stdarg_h

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif /* __GNUC_VA_LIST */

#ifndef __va_list__
typedef __gnuc_va_list va_list;
#endif /* __va_list__ */

#ifndef __va_funcs__
#define __va_funcs__
#define va_start(v, l) __builtin_va_start(v, l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v, l) __builtin_va_arg(v, l)
#endif /* __va_funcs__ */

#endif /* _libc_stdarg_h */