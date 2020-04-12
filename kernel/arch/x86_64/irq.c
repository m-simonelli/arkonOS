/*
 *  irq.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <arch/x86_64/8259_pic.h>
#include <arch/x86_64/idt.h>
#include <arch/x86_64/irq.h>
#include <arch/x86_64/isr.h>
#include <drivers/io/ports.h>
#include <k_log.h>
#include <stddef.h>

void irq_handler(int eno, regs_t regs) {
    isr_handler_t handler = int_handlers[eno];
    if (handler) handler(regs);
}