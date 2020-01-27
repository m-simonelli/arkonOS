#include <drivers/serial/serial.h>
#include <drivers/vga/vga.h>
#include <util/ascii_tools.h>
#include <mem/pmm.h>

//extern mem_ptr_u32_t e820_map;
void kmain() {
    /* Initialize VGA */
    vga_init();

    /* Initalize serial I/O */
    serial_init();

    /* Log a test message to vga */
    vga_log("kernel init\n");

    char map_val[sizeof(int_t)] = {0};
    map_val[0] = '0';
    map_val[1] = 'x';
    itoa((int_t)e820_map_addr, (char *)&map_val[2], 16);
    vga_print((char *)&map_val);
}