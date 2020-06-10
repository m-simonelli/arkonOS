/*
 *  init.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <arch/x86_64/idt.h>
#include <devices/io/kb.h>
#include <devices/io/serial/serial.h>
#include <devices/display/vga/vga.h>
#include <inttypes.h>
#include <k_log.h>
#include <kcmd.h>
#include <lib/rand.h>
#include <mm/e820.h>
#include <mm/pmm.h>

void kmain(int systime) {
    /* Initialize VGA */
    vga_init();

    k_printf("Welcome to ArkonOS 0.3.0-alpha, compiled on %s at %s\n", __DATE__,
             __TIME__);

    k_printf("Clocks since midnight: %d\n", systime);

    set_rand_algorithm(RAND_ALGORITHM_MT);
    srand(systime);
    k_printf("Random value: %d\n", rand());
    k_printf("Random value: %d\n", rand());
    k_printf("Random value: %d\n", rand());
    k_printf("Random value: %d\n", rand());
    k_printf("Random value: %d\n", rand());

    /* Initalize serial I/O */
    serial_init();

    /* init the e820 map, store in an array, print to output etc */
    init_e820(e820_map_addr);

    /* init physical memory manager with the size that we got from e820 */
    init_pmm(total_e820_size);

    init_idt();
    init_keyboard();

    kprintf(
        "Type 'help' for a list of commands.\nAnything else typed will be "
        "echoed back\n");
    /* print a console line */
    run_kcmd(NULL);
}