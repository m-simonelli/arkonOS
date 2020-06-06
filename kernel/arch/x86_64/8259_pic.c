/*
 *  8259_pic.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <arch/x86_64/8259_pic.h>
#include <devices/io/ports.h>
#include <inttypes.h>

void remap_8259_pic(int offset_master, int offset_slave) {
    uint8_t mask_master, mask_slave;

    mask_master = port_byte_in(PICMASTER_DATA);
    mask_slave = port_byte_in(PICSLAVE_DATA);

    port_byte_out(PICMASTER_COMMAND, ICW1_INIT | ICW1_ICW4);
    port_byte_out(PICSLAVE_COMMAND, ICW1_INIT | ICW1_ICW4);
    port_byte_out(PICMASTER_DATA, offset_master);
    port_byte_out(PICSLAVE_DATA, offset_slave);
    port_byte_out(PICMASTER_DATA, 4);
    port_byte_out(PICSLAVE_DATA, 2);

    port_byte_out(PICMASTER_DATA, ICW4_8086);
    port_byte_out(PICSLAVE_DATA, ICW4_8086);

    port_byte_out(PICMASTER_DATA, mask_master);
    port_byte_out(PICSLAVE_DATA, mask_slave);
}