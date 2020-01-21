#ifndef _inttypes_h
#define _inttypes_h
#include <conf.h>

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed int int16_t;
typedef unsigned int uint16_t;
typedef signed long int int32_t;
typedef unsigned long int uint32_t;

#ifdef KERN_TARGET_x86_64
typedef signed long long int int64_t;
typedef unsigned long long int uint64_t;
#endif /* KERN_TARGET_x86_64 */

typedef uint16_t count16_t;
typedef uint32_t count32_t;
typedef uint64_t count64_t;

#ifdef KERN_TARGET_x86_64
typedef count64_t count_t;
#else
typedef count32_t count_t;
#endif /* KERN_TARGET_x86_64 */

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#ifdef KERN_TARGET_x86_64
typedef int64_t int_t;
typedef uint64_t uint_t;
typedef uint64_t size_t;
typedef uint64_t mem_addr_t; /* use for pointer arithmetic casts, i.e.
                              *((mem_ptr_t)((mem_addr_t)addr+val)) */
#else
typedef int32_t int_t;
typedef uint32_t uint_t;
typedef uint32_t size_t;
typedef uint32_t mem_addr_t;
#endif /* KERN_TARGET_x86_64 */

typedef void* mem_ptr_t;
typedef void* symbol[];

typedef uint8_t* mem_ptr_u8_t;
typedef uint16_t* mem_ptr_u16_t;
typedef uint32_t* mem_ptr_u32_t;
typedef uint64_t* mem_ptr_u64_t;
typedef char* mem_ptr_char_t;

typedef u8 byte_t;
typedef u16 word_t;
typedef u32 dword_t;
typedef u64 qword_t;

#endif /* _inttypes_h */