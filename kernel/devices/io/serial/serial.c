/*
 *  serial.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <devices/io/serial/serial.h>
#include <devices/io/serial/uart.h>
#include <k_log.h>

char serial_read() {
    /* wait until data can be read */
    while (!is_data_ready()) {
    }
    /* return the byte read */
    return port_byte_in(COM_PORT);
}

void serial_write(port_byte_t b) {
    /* wait until tx buffer is empty, we can't send before this */
    while (!is_tx_empty()) {
    }
    /* tx buffer is empty, send byte */
    port_byte_out(COM_PORT, b);
#if KERN_TARGET_QEMU == 1
#if KERN_QEMU_DEBUG_PORT_ENABLED == 1
    port_byte_out(QEMU_DEBUG_PORT, b);
#endif /* KERN_QEMU_DEBUG_PORT_ENABLED */
#endif /* KERN_TARGET_QEMU */
}

void serial_init() {
    /* no interrupts */
    uart_disable_all_interrupts();
    /* 115200 baud */
    set_baud_divisor(BAUD_DIVISOR);
    /* eight bits per sent character */
    set_data_bits(UART_CHARACTER_EIGHT_BITS);
    /* no parity */
    set_parity(UART_PARITY_NONE);
    /* 14 byte FIFO buffer*/
    set_uart_fifo_interrupt_level(FOURTEEN_BYTE_FIFO);
    /* enable data transfer ready bit */
    uart_modem_enable_dtr();
    /* enable request to send bit */
    uart_modem_enable_rts();
    /* enable aux output 2 bit */
    uart_modem_enable_ao2();
    /* log to output */
    k_log("serial init\n");
}

void serial_print(const char *restrict serial_msg) {
    /* Write each char until null terminator */
    for (count_t i = 0; serial_msg[i] != 0; i++) serial_write(serial_msg[i]);
}