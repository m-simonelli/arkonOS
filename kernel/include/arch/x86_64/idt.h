/*
 *  idt.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _arch_x86_64_idt_h
#define _arch_x86_64_idt_h

#include <inttypes.h>

#define IDT_ENTRIES 256
#define K_CODE 0x8

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t flags;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} __attribute__((packed)) idt_gate_entry_t;

typedef struct {
    uint16_t sz;
    uint64_t addr;
} __attribute__((packed)) idt_descriptor_t;

extern idt_descriptor_t idt_descriptor;
extern idt_gate_entry_t idt_gates[IDT_ENTRIES];

void init_idt();

#endif