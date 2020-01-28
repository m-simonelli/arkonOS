#include <mem/pmm.h>
#include <drivers/vga/vga.h>

/* How big pages are, for the PMM this will be 4KiB */
#define PAGE_SIZE 4096
#define BLOCK_SIZE PAGE_SIZE
/* Each page has 2 bits, so 4 pages can be managed per byte */
#define PAGES_PER_BYTE 4
/* Handy macro to calculate bitmap size */
#define BITMAP_SIZE(mem_size) (mem_size/PAGE_SIZE/PAGES_PER_BYTE)

void init_pmm(size_t mem_size){
    vga_print_color("[pmm] ", VGA_COL_BACKGROUND_BLACK | VGA_COL_FOREGROUND_LIGHT_RED);
    vga_printf("mem_size: %#08llx\n", mem_size);
    vga_print_color("[pmm] ", VGA_COL_BACKGROUND_BLACK | VGA_COL_FOREGROUND_LIGHT_RED);
    vga_printf("bitmap size: %#08llx\n", BITMAP_SIZE(mem_size));
}