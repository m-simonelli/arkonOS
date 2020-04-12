/*
 *  isr.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _arch_x86_64_isr_h
#define _arch_x86_64_isr_h

#include <arch/x86_64/exception.h>
#include <arch/x86_64/idt.h>

typedef void (*isr_handler_t)(regs_t r);
extern isr_handler_t int_handlers[IDT_ENTRIES];

static inline void register_isr_handler(int isr, isr_handler_t handler) {
    int_handlers[isr] = handler;
}

#endif