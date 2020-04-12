/*
 *  exception.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _arch_x86_64_exception_h
#define _arch_x86_64_exception_h

#include <inttypes.h>

typedef struct {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
} regs_t;

void isr_div0(void);
void isr_debug(void);
void isr_nmi(void);
void isr_breakpoint(void);
void isr_overflow(void);
void isr_bound_range(void);
void isr_inv_opcode(void);
void isr_no_dev(void);
void isr_double_fault(void);
void isr_cp_overrun(void);
void isr_inv_tss(void);
void isr_no_segment(void);
void isr_ss_fault(void);
void isr_gpf(void);
void isr_page_fault(void);
void isr_x87_fp(void);
void isr_alignment_check(void);
void isr_machine_check(void);
void isr_simd_fp(void);
void isr_virt(void);
void isr_security(void);

void irq0(void);
void irq1(void);
void irq2(void);
void irq3(void);
void irq4(void);
void irq5(void);
void irq6(void);
void irq7(void);
void irq8(void);
void irq9(void);
void irq10(void);
void irq11(void);
void irq12(void);
void irq13(void);
void irq14(void);
void irq15(void);

#endif