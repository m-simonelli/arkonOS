/*
 *  exception.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <arch/x86_64/exception.h>
#include <k_log.h>

static const char *exception_names[] = {"Division by 0",
                                        "Debug",
                                        "NMI",
                                        "Breakpoint",
                                        "Overflow",
                                        "Bound range exceeded",
                                        "Invalid opcode",
                                        "Device not available",
                                        "Double fault",
                                        "Coprocessor segment overrun",
                                        "Invalid TSS",
                                        "Segment not present",
                                        "Stack-segment fault",
                                        "General protection fault",
                                        "Page fault",
                                        "???",
                                        "x87 exception",
                                        "Alignment check",
                                        "Machine check",
                                        "SIMD exception",
                                        "Virtualisation",
                                        "???",
                                        "???",
                                        "???",
                                        "???",
                                        "???",
                                        "???",
                                        "???",
                                        "???",
                                        "???",
                                        "Security"};

void exception_handler(int eno, regs_t *regs __attribute__((unused))) {
    k_log("recieved interrupt %d (%s)\n", eno, exception_names[eno]);
}