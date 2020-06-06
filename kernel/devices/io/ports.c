/*
 *  ports.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */
#include <devices/io/ports.h>

/*
Read 1 byte from port `port`
*/
port_byte_t port_byte_in(port_t port) {
    port_byte_t res;

    __asm__("in %%dx, %%al" : "=a"(res) : "d"(port));
    return res;
}

/*
Write 1 byte `data` to port `port`
*/
void port_byte_out(port_t port, port_byte_t data) {
    __asm__("out %%al, %%dx" : : "d"(port), "a"(data));
}

/*
Read 1 word (2 bytes) from port `port`
*/
port_word_t port_word_in(port_t port) {
    port_word_t res;

    __asm__("in %%dx, %%ax" : "=a"(res) : "d"(port));
    return res;
}

/*
Write 1 word (2 bytes) `data` to port `port`
*/
void port_word_out(port_t port, port_word_t data) {
    __asm__("out %%ax, %%dx" : : "d"(port), "a"(data));
}

/*
Force the CPU to wait for the current I/O to finish
*/
void io_wait() { port_byte_out(0x80, 0); }