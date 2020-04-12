/*
 *  inttypes.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _inttypes_h
#define _inttypes_h
#include <conf.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __linux__
#include <sys/types.h>
#else
#ifdef KERN_TARGET_x86_64
typedef signed long long int ssize_t;
#else
typedef signed long int ssize_t;
#endif /* KERN_TARGET_x86_64 */
#endif /* __linux__ */

typedef uint16_t count16_t;
typedef uint32_t count32_t;
#ifdef KERN_TARGET_x86_64
typedef uint64_t count64_t;
#endif /* KERN_TARGET_x86_64 */

typedef size_t count_t;

typedef ssize_t int_t;
typedef size_t uint_t;

typedef void* symbol[];

typedef uint8_t byte_t;
typedef uint16_t word_t;
typedef uint32_t dword_t;
#ifdef KERN_TARGET_x86_64
typedef uint64_t qword_t;
#endif /* KERN_TARGET_x86_64 */

typedef count_t iterator_t;

#define BYTE byte_t
#define WORD word_t
#define DWORD dword_t
#ifdef KERN_TARGET_x86_64
#define QWORD qword_t
#endif /* KERN_TARGET_x86_64 */

#endif /* _inttypes_h */