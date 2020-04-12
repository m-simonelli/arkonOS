/*
 *  init.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <arch/x86_64/idt.h>
#include <drivers/io/kb.h>
#include <drivers/serial/serial.h>
#include <drivers/vga/vga.h>
#include <inttypes.h>
#include <k_log.h>
#include <kcmd.h>
#include <libc/printf.h>
#include <mem/e820.h>
#include <mem/pmm.h>
#include <stddef.h>
#include <util/ascii_tools.h>

void kmain() {
    /* Initialize VGA */
    vga_init();

    k_printf("Welcome to ArkonOS 0.1.0-alpha, compiled on %s at %s\n", __DATE__, __TIME__);
    /* Initalize serial I/O */
    serial_init();

    /* init the e820 map, store in an array, print to output etc */
    init_e820(e820_map_addr);

    /* init physical memory manager with the size that we got from e820 */
    init_pmm(total_e820_size);

    init_idt();
    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");

    init_keyboard();
    run_kcmd(NULL);
}