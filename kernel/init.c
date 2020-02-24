#include <k_log.h>
#include <drivers/serial/serial.h>
#include <drivers/vga/vga.h>
#include <inttypes.h>
#include <libc/printf.h>
#include <mem/e820.h>
#include <mem/pmm.h>
#include <util/ascii_tools.h>

void kmain() {
    /* Initialize VGA */
    vga_init();

    k_printf("Welcome to ArkonOS, compiled on %s at %s\n",
            __DATE__, __TIME__);
    /* Initalize serial I/O */
    serial_init();

    /* init the e820 map, store in an array, print to output etc */
    init_e820(e820_map_addr);

    /* init physical memory manager with the size that we got from e820 */
    init_pmm(total_e820_size);
}