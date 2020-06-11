/*
 *  kcmd.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <k_log.h>
#include <mm/pmm.h>
#include <string.h>
#include <lib/klib.h>
#include <lib/rand.h>

void run_kcmd(char *cmd) {
    if (cmd) {
        if (strcmp(cmd, "hello") == 0) {
            kprintf("hello there!\n")
        } else if (strcmp(cmd, "help") == 0) {
            kprintf("Available commands: hello, help, page_alloc, end\n");
        } else if (strcmp(cmd, "page_alloc") == 0) {
            kprintf("allocated page at paddr: %#0*llx\n", 16,
                    pmm_alloc_block());
        } else if (strcmp(cmd, "end") == 0) {
            kprintf("goodbye!\n");
            asm volatile(
                "\n\tcli"
                "\n\thlt");
        } else if (strcmp(cmd, "rand") == 0){
            /* TODO:
                1. Implement string split
                2. malloc() for 1.
                3. Lower bound + upper bound after 1 and 2
            */
            kprintf("%d\n", rand());
        } else {
            kprintf("%s\n", cmd);
        }
    }
    kprintf("> ");
}