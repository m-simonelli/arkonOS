/*
 *  kb.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <arch/x86_64/isr.h>
#include <drivers/io/kb.h>
#include <drivers/io/ports.h>
#include <k_log.h>
#include <kcmd.h>
#include <mem/pmm.h>
#include <mem/string.h>
#include <panic.h>
#include <stddef.h>

#define SC_ENTER 0x1C
#define SC_BACKSPACE 0x0E
#define SC_MAX 57

char keybuf[PAGE_SIZE];
size_t current_input_size = 0;

const char scancode_ascii[] = {
    '?', '?', '1',  '2', '3',  '4', '5', '6',  '7', '8', '9', '0',
    '-', '=', '\b', '?', 'Q',  'W', 'E', 'R',  'T', 'Y', 'U', 'I',
    'O', 'P', '[',  ']', '?',  '?', 'A', 'S',  'D', 'F', 'G', 'H',
    'J', 'K', 'L',  ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M',  ',', '.',  '/', '?', '?',  '?', ' '};

void keyboard_callback(regs_t regs) {
    uint8_t sc = port_byte_in(0x60);
    if (sc > SC_MAX) return;
    if (sc == SC_ENTER) {
        kprintf("\n");
        run_kcmd((char *)&keybuf);
        bzero(keybuf, PAGE_SIZE);
        current_input_size = 0;
    } else {
        char c = scancode_ascii[(int)sc];
        if(sc != SC_BACKSPACE){
            keybuf[current_input_size++] = c;
        } else {
            keybuf[--current_input_size] = '\0';
        }
        kprintf("%c", c);
    }
}

void init_keyboard() { register_isr_handler(33, keyboard_callback); }
