/*
 *  isr.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <arch/x86_64/isr.h>

isr_handler_t int_handlers[IDT_ENTRIES] = {0};