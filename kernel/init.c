#include <drivers/serial/serial.h>
#include <drivers/vga/vga.h>
#include <inttypes.h>
#include <mem/e820.h>
#include <util/ascii_tools.h>

void kmain() {
    /* Initialize VGA */
    vga_init();

    /* Initalize serial I/O */
    serial_init();

    /* Log a test message to vga */
    vga_log("kernel init\n");

    /* init the e820 map, store in an array, print to vga etc */
    init_e820(e820_map_addr);
}