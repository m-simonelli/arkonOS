#include <drivers/serial/serial.h>
#include <drivers/vga/vga.h>

void kmain() {
    /* Initialize VGA */
    vga_init();

    /* Initalize serial I/O */
    serial_init();

    /* Log a test message to vga */
    vga_log("kernel init\n");
}