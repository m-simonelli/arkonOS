/*
 *  ssp.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

/*
 *  Stack Smashing Protection implementation
 */

#include <lib/panic.h>

unsigned long long *__stack_chk_guard;

__attribute__((noreturn)) void __stack_chk_fail(void){
    panic("Stack smashing detected\n");
}