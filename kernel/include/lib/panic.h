/*
 *  panic.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _panic_h
#define _panic_h

#include <conf.h>

extern __attribute__((noreturn)) void halt();
__attribute__((noreturn)) void _panic(char *pmsg, ...);

extern struct panic_dump p_regs;

extern void _panic_save_regs_to_p_regs();

#define panic(msg, ...)               \
    do {                              \
        _panic_save_regs_to_p_regs(); \
        _panic(msg, ##__VA_ARGS__);   \
    } while (0);

struct panic_dump {
    /* Address of panic */
    size_t ip;

    /* General purpose registers */
    size_t ax;
    size_t bx;
    size_t cx;
    size_t dx;
    size_t si;
    size_t di;
    size_t sp;
    size_t bp;

#ifdef KERN_TARGET_x86_64
    /* These are only available on x86_64 */
    size_t r8;
    size_t r9;
    size_t r10;
    size_t r11;
    size_t r12;
    size_t r13;
    size_t r14;
    size_t r15;
#endif /* KERN_TARGET_x86_64 */

    /* Useful control registers */
    size_t cr2;
    size_t cr3;
};

#endif /* _panic_h */