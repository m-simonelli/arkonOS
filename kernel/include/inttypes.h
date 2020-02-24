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

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#ifdef KERN_TARGET_x86_64
typedef uint64_t u64;
#endif /* KERN_TARGET_x86_64 */

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
#ifdef KERN_TARGET_x86_64
typedef int64_t i64;
#endif /* KERN_TARGET_x86_64 */

typedef ssize_t int_t;
typedef size_t uint_t;

typedef void* symbol[];

typedef u8 byte_t;
typedef u16 word_t;
typedef u32 dword_t;
#ifdef KERN_TARGET_x86_64
typedef u64 qword_t;
#endif /* KERN_TARGET_x86_64 */

typedef count_t iterator_t;

#define BYTE byte_t
#define WORD word_t
#define DWORD dword_t
#ifdef KERN_TARGET_x86_64
#define QWORD qword_t
#endif /* KERN_TARGET_x86_64 */

#endif /* _inttypes_h */