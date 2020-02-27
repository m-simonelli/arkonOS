/*
 *  panic.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the 
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <drivers/vga/vga.h>
#include <k_log.h>
#include <libc/printf.h>
#include <panic.h>
#include <stddef.h>

/* Allocate this at compile-time so that in the event of no memory left,
 * we can still print regs */
struct panic_dump p_regs = {0};

void _panic_printf(char *msg, ...) {
    va_list(ap);
    va_start(ap, msg);
    vsprintf(NULL, log_putchar, msg, ap);
    va_end(ap);
}

/* panic and halt */
__attribute__((noreturn)) void _panic(char *pmsg, ...) {
    va_list(ap);
    va_start(ap, pmsg);
    vsprintf(NULL, log_putchar, "\n*****PANIC*****\n", NULL);
    vsprintf(NULL, log_putchar, pmsg, ap);
    vsprintf(NULL, log_putchar, "\nRegister Dump:\n", NULL);
#ifdef KERN_TARGET_x86_64
    _panic_printf("RIP: %#0*llx\n", 16, p_regs.ip);

    _panic_printf("RAX: %#0*llx\n", 16, p_regs.ax);
    _panic_printf("RBX: %#0*llx\n", 16, p_regs.bx);
    _panic_printf("RCX: %#0*llx\n", 16, p_regs.cx);
    _panic_printf("RDX: %#0*llx\n", 16, p_regs.dx);

    _panic_printf("RSI: %#0*llx\n", 16, p_regs.si);
    _panic_printf("RDI: %#0*llx\n", 16, p_regs.di);

    _panic_printf("RSP: %#0*llx\n", 16, p_regs.sp);
    _panic_printf("RBP: %#0*llx\n", 16, p_regs.bp);

    _panic_printf("R8: %#0*llx\n", 16, p_regs.r8);
    _panic_printf("R9: %#0*llx\n", 16, p_regs.r9);
    _panic_printf("R10: %#0*llx\n", 16, p_regs.r10);
    _panic_printf("R11: %#0*llx\n", 16, p_regs.r11);
    _panic_printf("R12: %#0*llx\n", 16, p_regs.r12);
    _panic_printf("R13: %#0*llx\n", 16, p_regs.r13);
    _panic_printf("R14: %#0*llx\n", 16, p_regs.r14);
    _panic_printf("R15: %#0*llx\n", 16, p_regs.r15);

    _panic_printf("CR2: %#0*llx\n", 16, p_regs.cr2);
    _panic_printf("CR3: %#0*llx\n", 16, p_regs.cr3);
#else  /* !KERN_TARGET_x86_64 */
    /* TODO: 32 bit panic reg dumping */
#endif /* KERN_TARGET_x86_64 */
    vsprintf(NULL, log_putchar, "*****END PANIC*****", NULL);
    va_end(ap);
    halt();
}