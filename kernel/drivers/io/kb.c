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

#define SC_LSHIFT_DOWN 0x2A
#define SC_RSHIFT_DOWN 0x36
#define SC_LSHIFT_UP 0xAA
#define SC_RSHIFT_UP 0xB6

#define SC_CAPSLOCK_DOWN 0x3A
#define SC_CAPSLOCK_UP 0xBA

char keybuf[PAGE_SIZE];
size_t current_input_size = 0;

static char sc_nomod[] = {
    '\0', '\e', '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8', '9', '0',
    '-',  '=',  '\b', '\t', 'q',  'w',  'e',  'r',  't',  'y', 'u', 'i',
    'o',  'p',  '[',  ']',  '\n', '\0', 'a',  's',  'd',  'f', 'g', 'h',
    'j',  'k',  'l',  ';',  '\'', '`',  '\0', '\\', 'z',  'x', 'c', 'v',
    'b',  'n',  'm',  ',',  '.',  '/',  '\0', '\0', '\0', ' '};

static char sc_shift[] = {
    '\0', '\e', '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*', '(', ')',
    '_',  '+',  '\b', '\t', 'Q',  'W',  'E',  'R',  'T',  'Y', 'U', 'I',
    'O',  'P',  '{',  '}',  '\n', '\0', 'A',  'S',  'D',  'F', 'G', 'H',
    'J',  'K',  'L',  ':',  '"',  '~',  '\0', '|',  'Z',  'X', 'C', 'V',
    'B',  'N',  'M',  '<',  '>',  '?',  '\0', '\0', '\0', ' '};

static char sc_caps[] = {
    '\0', '\e', '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8', '9', '0',
    '-',  '=',  '\b', '\t', 'Q',  'W',  'E',  'R',  'T',  'Y', 'U', 'I',
    'O',  'P',  '[',  ']',  '\n', '\0', 'A',  'S',  'D',  'F', 'G', 'H',
    'J',  'K',  'L',  ';',  '\'', '`',  '\0', '\\', 'Z',  'X', 'C', 'V',
    'B',  'N',  'M',  ',',  '.',  '/',  '\0', '\0', '\0', ' '};

static char sc_shift_caps[] = {
    '\0', '\e', '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*', '(', ')',
    '_',  '+',  '\b', '\t', 'q',  'w',  'e',  'r',  't',  'y', 'u', 'i',
    'o',  'p',  '{',  '}',  '\n', '\0', 'a',  's',  'd',  'f', 'g', 'h',
    'j',  'k',  'l',  ':',  '"',  '~',  '\0', '|',  'z',  'x', 'c', 'v',
    'b',  'n',  'm',  '<',  '>',  '?',  '\0', '\0', '\0', ' '};

static char *current_sc_set = sc_nomod;

void keyboard_callback(regs_t regs __attribute__((unused))) {
    uint8_t sc = port_byte_in(0x60);
    if (sc == SC_LSHIFT_DOWN || sc == SC_RSHIFT_DOWN) {
        if (current_sc_set == sc_caps)
            current_sc_set = sc_shift_caps;
        else
            current_sc_set = sc_shift;
    } else if (sc == SC_LSHIFT_UP || sc == SC_RSHIFT_UP) {
        if (current_sc_set == sc_shift_caps)
            current_sc_set = sc_caps;
        else
            current_sc_set = sc_nomod;
    } else if (sc == SC_CAPSLOCK_DOWN) {
        if (current_sc_set == sc_shift)
            current_sc_set = sc_shift_caps;
        else if (current_sc_set == sc_shift_caps)
            current_sc_set = sc_shift;
        else if (current_sc_set == sc_caps)
            current_sc_set = sc_nomod;
        else
            current_sc_set = sc_caps;
    } else if (sc == SC_ENTER) {
        kprintf("\n");
        run_kcmd((char *)&keybuf);
        bzero(keybuf, PAGE_SIZE);
        current_input_size = 0;
    } else if (sc <= SC_MAX) {
        char c = current_sc_set[(int)sc];
        if (sc != SC_BACKSPACE) {
            keybuf[current_input_size++] = c;
        } else {
            if (current_input_size == 0) return;
            keybuf[--current_input_size] = '\0';
        }
        kprintf("%c", c);
    }
}

void init_keyboard() { register_isr_handler(33, keyboard_callback); }
