#ifndef _drivers_serial_serial_h
#define _drivers_serial_serial_h
#include <conf.h>
#include <drivers/io/ports.h>
#include <inttypes.h>

/* Whether to log messages to serial or not */
#define LOG_TO_SERIAL 1

/*
The serial baud divisor, this is used to calculate the baud rate as follows:
    baud_rate = 115200 / BAUD_DIVISOR
The divisor has to be >0
*/
#define BAUD_DIVISOR 1

/*!
    @function serial_read
    Read 1 byte from the serial port set by set_serial_port

    @return
    1 byte read from the set serial port
!*/
char serial_read();

/*!
    @function serial_read
    Write 1 byte to the serial port set by set_serial_port

    @param b
    The byte to be written
!*/
void serial_write(port_byte_t b);

/*!
    @function serial_init
    Initialize serial transfer
!*/
void serial_init();

/*!
    @function serial_print
    Print a message to serial

    @param serial_msg
    The message to be printed to serial
!*/
void serial_print(mem_ptr_char_t serial_msg);
#endif /* _drivers_serial_serial_h */