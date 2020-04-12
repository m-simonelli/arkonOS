/*
 *  kcmd.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <k_log.h>
#include <mem/string.h>
#include <mem/pmm.h>

void run_kcmd(char *cmd) { 
    if(cmd){
        if(strcmp(cmd, "HELLO") == 0){
            kprintf("hello there!\n")
        } else if (strcmp(cmd, "HELP") == 0){
            kprintf("Available commands: HELLO, PAGE-ALLOC, END\n");    
        } else if (strcmp(cmd, "PAGE-ALLOC") == 0){
            kprintf("allocated page at paddr: %#0*llx\n", 16, pmm_alloc_block());
        } else if (strcmp(cmd, "END") == 0){
            kprintf("goodbye!\n");
            asm volatile(
                "\n\tcli"
                "\n\thlt"
            );
        } else {
            kprintf("%s\n", cmd);
        }
    }
    kprintf("> "); 
}