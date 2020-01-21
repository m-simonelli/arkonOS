#include <drivers/serial/uart.h>
/*
TODO: Check for COM port addresses instead of relying on hardcoded
*/
port_t COM1 = 0x3F8;
port_t COM2 = 0x2F8;

#if KERN_TARGET_QEMU == 1
port_t QEMU_DEBUG_PORT = QEMU_DEBUG_PORT_ADDRESS;
#endif

/* Use com port 1 */
port_t COM_PORT = 0x3F8;

/* We can't read from FIFO control reg, store it's status here */
static uint8_t fifo_enabled = 0;

void set_serial_port(port_t port) { COM_PORT = port; }

/* Baud rate*/
void set_baud_divisor(uint16_t divisor) {
    /* Check that DLAB bit is enabled, if not enable it*/
    if ((read_line_control() & 0x80) == 0) {
        enable_dlab();
    }
    /* Write the LSB to the divisor LSB register */
    port_byte_out(COM_PORT + OFF_DIVISOR_LSB, divisor & 0xFF);
    /* Write the MSB to the divisor MSB register */
    port_byte_out(COM_PORT + OFF_DIVISOR_MSB, divisor >> 8);
    /* Disable DLAB */
    disable_dlab();
}

/* FIFO Control / int ID reg*/
/* Read the interrupt identification register*/
port_byte_t read_iid() {
    return port_byte_in(COM_PORT + OFF_INT_ID_FIFO_CTRL_REG);
}

/* Clear the FIFO rx buffer */
void uart_fifo_clear_rx() {
    port_byte_out(COM_PORT + OFF_INT_ID_FIFO_CTRL_REG,
                  FIFO_CLR_RX | (fifo_enabled & 0x1));
}

/* Clear the FIFO tx buffer */
void uart_fifo_clear_tx() {
    port_byte_out(COM_PORT + OFF_INT_ID_FIFO_CTRL_REG,
                  FIFO_CLR_TX | (fifo_enabled & 0x1));
}

/* Set how many bytes need to be in the tx/rx buffer to trigger an interrupt */
void set_uart_fifo_interrupt_level(
    enum uart_fifo_interrupt_trigger_levels int_lvl) {
    if (!fifo_enabled) uart_enable_fifo(1);
    port_byte_out(COM_PORT + OFF_INT_ID_FIFO_CTRL_REG, int_lvl | 0x1);
}

/* Enable FIFO, clear the rx and tx buffers if requested */
void uart_enable_fifo(uint8_t clr_buf) {
    fifo_enabled = 1;
    port_byte_out(COM_PORT + OFF_INT_ID_FIFO_CTRL_REG, 0x1);
    if (clr_buf) {
        uart_fifo_clear_rx();
        uart_fifo_clear_tx();
    }
}

/* Disable FIFO */
void uart_disable_fifo() {
    fifo_enabled = 0;
    port_byte_out(COM_PORT + OFF_INT_ID_FIFO_CTRL_REG, 0x0);
}

/* Check if FIFO is enabled */
enum uart_fifo_status uart_is_fifo_enabled() {
    return (port_byte_in(COM_PORT + OFF_INT_ID_FIFO_CTRL_REG) & 0xC0) >> 6;
}

/* Modem control register */
/* Read the value of the modem control register */
port_byte_t read_modem_ctrl() {
    return port_byte_in(COM_PORT + OFF_MODEM_CTRL_REG);
}

/* Enable data terminal/transfer ready */
void uart_modem_enable_dtr() {
    port_byte_out(COM_PORT + OFF_MODEM_CTRL_REG, read_modem_ctrl() | 0x1);
}

/* Enable request to send */
void uart_modem_enable_rts() {
    port_byte_out(COM_PORT + OFF_MODEM_CTRL_REG, read_modem_ctrl() | 0x2);
}

/* Disable data terminal/transfer ready */
void uart_modem_disable_dtr() {
    port_byte_out(COM_PORT + OFF_MODEM_CTRL_REG, read_modem_ctrl() & ~(0x1));
}

/* Disable request to send */
void uart_modem_disable_rts() {
    port_byte_out(COM_PORT + OFF_MODEM_CTRL_REG, read_modem_ctrl() & ~(0x2));
}

/* Enable ao1 */
void uart_modem_enable_ao1() {
    port_byte_out(COM_PORT + OFF_MODEM_CTRL_REG, read_modem_ctrl() | 0x4);
}

/* Enable ao2 */
void uart_modem_enable_ao2() {
    port_byte_out(COM_PORT + OFF_MODEM_CTRL_REG, read_modem_ctrl() | 0x8);
}

/* Disable ao1 */
void uart_modem_disable_ao1() {
    port_byte_out(COM_PORT + OFF_MODEM_CTRL_REG, read_modem_ctrl() & ~(0x4));
}

/* Disable ao2 */
void uart_modem_disable_ao2() {
    port_byte_out(COM_PORT + OFF_MODEM_CTRL_REG, read_modem_ctrl() & ~(0x8));
}

/* Enable loopback mode */
void uart_modem_enable_lbm() {
    port_byte_out(COM_PORT + OFF_MODEM_CTRL_REG, read_modem_ctrl() | 0x10);
}

/* Enable autoflow control */
void uart_modem_enable_afc() {
    port_byte_out(COM_PORT + OFF_MODEM_CTRL_REG, read_modem_ctrl() | 0x20);
}

/* Disable loopback mode */
void uart_modem_disable_lbm() {
    port_byte_out(COM_PORT + OFF_MODEM_CTRL_REG, read_modem_ctrl() & ~(0x10));
}

/* Disable autoflow control */
void uart_modem_disable_afc() {
    port_byte_out(COM_PORT + OFF_MODEM_CTRL_REG, read_modem_ctrl() & ~(0x20));
}

/* Interrupt enable register */
/* Disable all interrupts */
void uart_disable_all_interrupts() {
    port_byte_out(COM_PORT + OFF_INT_E_REG, 0x00);
}

/* Enable all interrupts */
void uart_enable_all_interrupts() {
    port_byte_out(COM_PORT + OFF_INT_E_REG, INT_DATA_AVAILABLE | INT_TX_EMPTY |
                                                INT_ERROR | INT_STATUS_CHANGE |
                                                INT_16750_SLEEP_MODE |
                                                INT_16750_LOW_PWR_MODE);
}

/*
Set provided interrupts, values from `enum uart_interrupt_enable_register`
can be bitwise OR'd and given here
*/
void uart_set_interrupts(uart_interrupt_t interrupts) {
    /* Get low 6 bits of interrupt, top 2 bits are reserved so don't write*/
    port_byte_out(COM_PORT + OFF_INT_E_REG, interrupts & 0x3F);
}

/* Line control register */
/* Enable DLAB for setting baud divisor */
void enable_dlab() {
    port_byte_out(COM_PORT + OFF_LINE_CTRL_REG, read_line_control() | 0x80);
}

/* Disable DLAB */
void disable_dlab() {
    port_byte_out(COM_PORT + OFF_LINE_CTRL_REG, read_line_control() & ~(0x80));
}

/* Read line control register */
port_byte_t read_line_control() {
    return port_byte_in(COM_PORT + OFF_LINE_CTRL_REG);
}

/* Set parity, defined in uart.h */
void set_parity(enum uart_parity parity) {
    port_byte_out(COM_PORT + OFF_LINE_CTRL_REG,
                  (read_line_control() & ~PARITY_BITS) | parity);
}

/* Set stop bits, defined in uart.h*/
void set_stop_bits(enum uart_stop_bits stop_bits) {
    port_byte_out(COM_PORT + OFF_LINE_CTRL_REG,
                  (read_line_control() & ~STOP_BITS) | stop_bits);
}

/* Set data bits, defined in uart.h */
void set_data_bits(enum uart_character_bits data_bits) {
    port_byte_out(COM_PORT + OFF_LINE_CTRL_REG,
                  (read_line_control() & ~DATA_BITS) | data_bits);
}

/* Line status register */

/* Read line status register */
port_byte_t read_line_status() {
    return port_byte_in(COM_PORT + OFF_LINE_STAT_REG);
}

/* Return 1 if data can be read, else 0 */
uint8_t is_data_ready() { return read_line_status() & DATA_READY; }

/* Return 1 if ready to send data, else 0 */
uint8_t is_tx_empty() { return read_line_status() & TX_REG_EMPTY; }

/* Check the errors and return them, else 0 */
uart_err_t check_line_errors() {
    return port_byte_in(COM_PORT + OFF_LINE_STAT_REG) &
           (OVERRUN_ERR | PARITY_ERR | FRAMING_ERR | IMP_ERROR);
}