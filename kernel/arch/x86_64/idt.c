/*
 *  idt.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <arch/x86_64/8259_pic.h>
#include <arch/x86_64/exception.h>
#include <arch/x86_64/idt.h>
#include <arch/x86_64/isr.h>
#include <k_log.h>
#include <conf.h>

idt_descriptor_t idt_descriptor;
idt_gate_entry_t idt_gates[IDT_ENTRIES] = {0};

void register_interrupt_handler(int n, void *handler, uint8_t ist,
                                uint8_t flags) {
    uint64_t func_addr = (uint64_t)handler;

    idt_gates[n].offset_low = (uint16_t)func_addr;
    idt_gates[n].offset_mid = (uint16_t)(func_addr >> 16);
    idt_gates[n].offset_high = (uint32_t)(func_addr >> 32);
    idt_gates[n].flags = flags;
    idt_gates[n].selector = K_CODE;
    idt_gates[n].ist = ist;
    idt_gates[n].zero = 0;
}

void init_idt() {
    /* Exception handlers */
    register_interrupt_handler(0x0, isr_div0, 0, 0x8e);
    register_interrupt_handler(0x1, isr_debug, 0, 0x8e);
    register_interrupt_handler(0x2, isr_nmi, 0, 0x8e);
    register_interrupt_handler(0x3, isr_breakpoint, 0, 0x8e);
    register_interrupt_handler(0x4, isr_overflow, 0, 0x8e);
    register_interrupt_handler(0x5, isr_bound_range, 0, 0x8e);
    register_interrupt_handler(0x6, isr_inv_opcode, 0, 0x8e);
    register_interrupt_handler(0x7, isr_no_dev, 0, 0x8e);
    register_interrupt_handler(0x8, isr_double_fault, 1, 0x8e);
    register_interrupt_handler(0xa, isr_inv_tss, 0, 0x8e);
    register_interrupt_handler(0xb, isr_no_segment, 0, 0x8e);
    register_interrupt_handler(0xc, isr_ss_fault, 0, 0x8e);
    register_interrupt_handler(0xd, isr_gpf, 0, 0x8e);
    register_interrupt_handler(0xe, isr_page_fault, 0, 0x8e);
    register_interrupt_handler(0x10, isr_x87_fp, 0, 0x8e);
    register_interrupt_handler(0x11, isr_alignment_check, 0, 0x8e);
    register_interrupt_handler(0x12, isr_machine_check, 0, 0x8e);
    register_interrupt_handler(0x13, isr_simd_fp, 0, 0x8e);
    register_interrupt_handler(0x14, isr_virt, 0, 0x8e);
    register_interrupt_handler(0x1e, isr_security, 0, 0x8e);

    remap_8259_pic(0x20, 0x28);

    register_interrupt_handler(0x20, irq0, 0, 0x8e);
    register_interrupt_handler(0x21, irq1, 0, 0x8e);
    register_interrupt_handler(0x22, irq2, 0, 0x8e);
    register_interrupt_handler(0x23, irq3, 0, 0x8e);
    register_interrupt_handler(0x24, irq4, 0, 0x8e);
    register_interrupt_handler(0x25, irq5, 0, 0x8e);
    register_interrupt_handler(0x26, irq6, 0, 0x8e);
    register_interrupt_handler(0x27, irq7, 0, 0x8e);
    register_interrupt_handler(0x28, irq8, 0, 0x8e);
    register_interrupt_handler(0x29, irq9, 0, 0x8e);
    register_interrupt_handler(0x2a, irq10, 0, 0x8e);
    register_interrupt_handler(0x2b, irq11, 0, 0x8e);
    register_interrupt_handler(0x2c, irq12, 0, 0x8e);
    register_interrupt_handler(0x2d, irq13, 0, 0x8e);
    register_interrupt_handler(0x2e, irq14, 0, 0x8e);
    register_interrupt_handler(0x2f, irq15, 0, 0x8e);

#if KERN_VERBOSITY >= 2
    k_log("registered interrupt handlers\n");
#endif 

    idt_descriptor_t idt_desc = {sizeof(idt_gates) - 1, (uint64_t)idt_gates};

    asm volatile("lidt %0" : : "m"(idt_desc));
    asm volatile("sti");

#if KERN_VERBOSITY >= 2
    k_log("loaded IDT\n");
#endif
}