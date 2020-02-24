#include <drivers/vga/vga.h>
#include <libc/printf.h>
#include <panic.h>
#include <stddef.h>
#include <k_log.h>

/* Allocate this at compile-time so that in the event of no memory left,
 * we can still print regs */
struct panic_dump p_regs = {0};

/* panic and halt */
__attribute__((noreturn)) void _panic(char *pmsg, ...) {
    va_list(ap);
    va_start(ap, pmsg);
    vsprintf(NULL, log_putchar, "\n*****PANIC*****\n", NULL);
    vsprintf(NULL, log_putchar, pmsg, NULL);
    vsprintf(NULL, log_putchar, "\nRegister Dump:\n", NULL);
#ifdef KERN_TARGET_x86_64
    vga_printf("RIP: %#0*llx\n", 16, p_regs.ip);

    vga_printf("RAX: %#0*llx\n", 16, p_regs.ax);
    vga_printf("RBX: %#0*llx\n", 16, p_regs.bx);
    vga_printf("RCX: %#0*llx\n", 16, p_regs.cx);
    vga_printf("RDX: %#0*llx\n", 16, p_regs.dx);

    vga_printf("RSI: %#0*llx\n", 16, p_regs.si);
    vga_printf("RDI: %#0*llx\n", 16, p_regs.di);

    vga_printf("RSP: %#0*llx\n", 16, p_regs.sp);
    vga_printf("RBP: %#0*llx\n", 16, p_regs.bp);


    vga_printf("R8: %#0*llx\n", 16, p_regs.r8);
    vga_printf("R9: %#0*llx\n", 16, p_regs.r9);
    vga_printf("R10: %#0*llx\n", 16, p_regs.r10);
    vga_printf("R11: %#0*llx\n", 16, p_regs.r11);
    vga_printf("R12: %#0*llx\n", 16, p_regs.r12);
    vga_printf("R13: %#0*llx\n", 16, p_regs.r13);
    vga_printf("R14: %#0*llx\n", 16, p_regs.r14);
    vga_printf("R15: %#0*llx\n", 16, p_regs.r15);

    vga_printf("CR2: %#0*llx\n", 16, p_regs.cr2);
    vga_printf("CR3: %#0*llx\n", 16, p_regs.cr3);
#else  /* !KERN_TARGET_x86_64 */
    /* TODO: 32 bit panic reg dumping */
#endif /* KERN_TARGET_x86_64 */
    vsprintf(NULL, log_putchar, "*****END PANIC*****", NULL);
    va_end(ap);
    halt();
}