/*
 *  8259_pic.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _arch_x86_64_8259_pic_h
#define _arch_x86_64_8259_pic_h

#define PIC1 0x20
#define PIC2 0xA0
#define PICMASTER PIC1
#define PICSLAVE PIC2

#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)

#define PICMASTER_COMMAND PICMASTER
#define PICMASTER_DATA (PICMASTER + 1)
#define PICSLAVE_COMMAND PICSLAVE
#define PICSLAVE_DATA (PICSLAVE + 1)

#define PIC_EOI 0x20

#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT 0x10

#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER 0x0C
#define ICW4_SFNM 0x10

void remap_8259_pic(int offset_master, int offset_slave);

#endif