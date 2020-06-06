/*
 *  uart.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _drivers_serial_uart_h
#define _drivers_serial_uart_h
/* TODO: probe to check the address of com ports */
#include <conf.h>
#include <devices/io/ports.h>
#include <inttypes.h>

typedef uint8_t uart_interrupt_t;
typedef uint8_t uart_err_t;

extern port_t COM1;
extern port_t COM2;

extern port_t COM_PORT;

#if KERN_TARGET_QEMU == 1
#define QEMU_DEBUG_PORT_ADDRESS 0xe9
extern port_t QEMU_DEBUG_PORT;
#endif

/* Register offsets */
/* reading from here reads from rx buf, writing writes to the tx buf */
static const uint8_t OFF_DATA_REG = 0;
/*(e|dis)able interrupts */
static const uint8_t OFF_INT_E_REG = 1;
/* least significant byte of baud divisor if DLAB is set (see line_ctrl_reg) */
static const uint8_t OFF_DIVISOR_LSB = 0;
/* most significant byte of baud divisor if DLAB is set */
static const uint8_t OFF_DIVISOR_MSB = 1;
/* interrupt identification and FIFO regs */
static const uint8_t OFF_INT_ID_FIFO_CTRL_REG = 2;
/* line control reg, most significant bit is DLAB */
static const uint8_t OFF_LINE_CTRL_REG = 3;
/* modem control register */
static const uint8_t OFF_MODEM_CTRL_REG = 4;
/* line status */
static const uint8_t OFF_LINE_STAT_REG = 5;
/* modem status */
static const uint8_t OFF_MODEM_STAT_REG = 6;
/* scratch register */
static const uint8_t OFF_SCRATCH_REG = 7;

/*
how many bits per character, less is faster. for ascii use 7
write to 2 least significant bits of line ctrl register
*/
enum uart_character_bits {
    UART_CHARACTER_FIVE_BITS,
    UART_CHARACTER_SIX_BITS,
    UART_CHARACTER_SEVEN_BITS,
    UART_CHARACTER_EIGHT_BITS
};

/*
how many zero bits are sent after each char
write to bit 2 of line ctrl reg
*/
enum uart_stop_bits { STOP_BITS_ONE, STOP_BITS_TWO };

/*
parity bits
UART_PARITY_NONE - no parity
UART_PARITY_ODD - parity bit should result in an odd amount of set bits in the
character UART_PARITY_EVEN - same as above but an even amount UART_PARITY_MARK -
parity bit is always 1 UART_PARITY_SPACE - parity bit is always 0
*/
enum uart_parity {
    UART_PARITY_NONE,
    UART_PARITY_ODD = 1,
    UART_PARITY_EVEN = 3,
    UART_PARITY_MARK = 5,
    UART_PARITY_SPACE = 7
};

/* which interrupts to enable */
enum uart_interrupt_enable_register {
    INT_DATA_AVAILABLE = 1 << 0,
    INT_TX_EMPTY = 1 << 1,
    INT_ERROR = 1 << 2,
    INT_STATUS_CHANGE = 1 << 3,
    INT_16750_SLEEP_MODE = 1 << 4,
    INT_16750_LOW_PWR_MODE = 1 << 5
};

/* these are set by the uart controller */
enum uart_line_status_reg {
    /* set if: */
    DATA_READY = 1 << 0,   /* data can be read */
    OVERRUN_ERR = 1 << 1,  /* data loss occurred */
    PARITY_ERR = 1 << 2,   /* incorrect parity */
    FRAMING_ERR = 1 << 3,  /* missing stop bit */
    BREAK_IND = 1 << 4,    /* break in data input */
    TX_REG_EMPTY = 1 << 5, /* tx buffer is empty, ready for sending */
    TX_EMPTY = 1 << 6,     /* transmitter is not doing anything */
    IMP_ERROR = 1 << 7     /* error with a word in rx buffer */
};

/* Bits for controlling FIFO */
enum uart_fifo_bits {
    FIFO_ENABLE_DISABLE = 1 << 0, /* The fifo enable bit */
    FIFO_CLR_RX = 1 << 1,         /* If set, the uart controller will clear
                                   * the rx buffer */
    FIFO_CLR_TX = 1 << 2,         /* If set, the uart controller will clear
                                   * the tx buffer */
    FIFO_DMA_MODE = 1 << 3,       /* The DMA enable bit */
    FIFO_16750_64_BYTE = 1 << 5,  /* 64 byte fifo on 16750. If not 16750
                                   * this is reserved */
    FIFO_INT_TRIG_LVL = 1 << 6 | 1 << 7 /* Interrupt trigger level bits */
};

/* How many bytes it takes for uart to trigger an interrupt */
enum uart_fifo_interrupt_trigger_levels {
    ONE_BYTE_FIFO = 0b00 << 6,
    FOUR_BYTE_FIFO = 0b01 << 6,
    EIGHT_BYTE_FIFO = 0b10 << 6,
    FOURTEEN_BYTE_FIFO = 0b11 << 6
};

/* FIFO status */
enum uart_fifo_status { NO_FIFO, UNUSABLE_FIFO, FIFO_ENABLED };

/* The bits that control data bits, stop bits etc */
enum uart_line_ctrl_reg_bits {
    DATA_BITS = 0b11,
    STOP_BITS = 0b100,
    PARITY_BITS = 0b111000,
    DLAB_BIT = 0b1000000
};

/* Bits that set dtr, rts etc */
enum uart_modem_control_bits {
    MODEM_DTR = 1 << 0, /* data transfer ready */
    MODEM_RTS = 1 << 1, /* req to send */
    MODEM_AO1 = 1 << 2, /* aux output 1 */
    MODEM_AO2 = 1 << 3, /* aux output 2 */
    MODEM_LBM = 1 << 4, /* loopback mode */
    MODEM_AFC = 1 << 5  /* autoflow control (16750) */
};

/* All of these functions have explanations in uart.c */
void set_serial_port(port_t port);
void set_baud_divisor(uint16_t divisor);
void uart_disable_all_interrupts();
void uart_enable_all_interrupts();
void uart_set_interrupts(uint8_t interrupts);
void enable_dlab();
void disable_dlab();
port_byte_t read_line_control();
void set_parity(enum uart_parity parity);
void set_stop_bits(enum uart_stop_bits stop_bits);
void set_data_bits(enum uart_character_bits data_bits);
port_byte_t read_line_status();
uint8_t is_data_ready();
uint8_t is_tx_empty();
uart_err_t check_line_errors();
port_byte_t read_iid();
void set_uart_fifo_interrupt_level(
    enum uart_fifo_interrupt_trigger_levels int_lvl);
void uart_enable_fifo(uint8_t clr_buf);
void uart_disable_fifo();
enum uart_fifo_status uart_is_fifo_enabled();
void uart_fifo_clear_rx();
void uart_fifo_clear_tx();
port_byte_t read_modem_ctrl();
void uart_modem_enable_dtr();
void uart_modem_enable_rts();
void uart_modem_disable_dtr();
void uart_modem_disable_rts();
void uart_modem_enable_ao1();
void uart_modem_enable_ao2();
void uart_modem_disable_ao1();
void uart_modem_disable_ao2();
void uart_modem_enable_lbm();
void uart_modem_enable_afc();
void uart_modem_disable_lbm();
void uart_modem_disable_afc();
#endif /* _drivers_serial_uart_h */