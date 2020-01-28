#include <drivers/vga/vga.h>
#include <mem/e820.h>

/*  The "unknown" strings are so that the corresponding string
    can be addressed by the type value given in the e820 entry  */
char *e820_region_type_strings[] = {"Unknown (0)",
                                    "Useable",
                                    "Reserved",
                                    "ACPI Reclaimable",
                                    "ACPI Non volatile",
                                    "Bad memory",
                                    "PRAM",
                                    "PMEM",
                                    "Unknown (8)",
                                    "Unknown (9)",
                                    "Unknown (10)",
                                    "Unknown (11)",
                                    "PRAM"};

mem_ptr_t e820_map_addr = NULL_PTR;

e820_entry_t e820_entries[E820_MAX] = {0};

e820_entry_t largest_contiguous_region;
size_t total_e820_size;

void init_e820(mem_ptr_t e820_addr) {
    /* Iterate over all entries and log them to VGA */
    for (count_t i = 0; i < *((dword_t *)e820_addr); i++) {
        /* Store each entry in an array */
        e820_entries[i] =
            (e820_entry_t){.base = *(u64 *)(e820_addr + 4 + (24 * i)),
                           .length = *(u64 *)(e820_addr + 12 + (24 * i)),
                           .end = e820_entries[i].base + e820_entries[i].length,
                           .type = *(u32 *)(e820_addr + 20 + (24 * i))};
        /* Print the entry */
        vga_print_color("[e820] ",
                        VGA_COL_BACKGROUND_BLACK | VGA_COL_FOREGROUND_CYAN);
        vga_print("start: ");
        vga_print_u64(e820_entries[i].base, 16);
        vga_print("    length: ");
        vga_print_u64(e820_entries[i].length, 16);
        vga_print("    type: ");
        vga_print(e820_region_type_strings[e820_entries[i].type]);
        vga_print_char('\n', 0);
    }
    /*
        Get the largest contiguous region of physical memory to
        make sure pmm bitmap doesn't overwrite anything important
    */
    largest_contiguous_region = get_longest_contiguous_e820_region(0);
    /* Print the largest contiguous region */
    vga_print_color("[e820] ",
                    VGA_COL_BACKGROUND_BLACK | VGA_COL_FOREGROUND_CYAN);
    vga_print("Largest Contiguous Region:");
    vga_print("\n         start:  ");
    vga_print_u64(largest_contiguous_region.base, 16);
    vga_print("\n         end:    ");
    vga_print_u64(largest_contiguous_region.end, 16);
    vga_print("\n         length: ");
    vga_print_u64(largest_contiguous_region.length, 16);
    vga_print("\n         type:   ");
    vga_print(e820_region_type_strings[largest_contiguous_region.type]);
    vga_print_char('\n', 0);

    total_e820_size = get_total_e820_size();
    vga_print_color("[e820] ", VGA_COL_BACKGROUND_BLACK | VGA_COL_FOREGROUND_CYAN);
    vga_print("Total size: ");
    vga_print_u64(total_e820_size, 16);
    vga_print_char('\n', 0);
}

e820_entry_t get_longest_contiguous_e820_region(uint8_t skip) {
    /* If skipping more than E820_MAX entries, return an empty entry */
    if (skip > E820_MAX) return (e820_entry_t){0, 0, 0, E820_UNKNOWN};
    /* Start with an entry with length 0 */
    e820_entry_t ret = {0, 0, 0, E820_UNKNOWN};
    /* Iterate over all stored entries */
    for (count_t i = skip; i < E820_MAX; i++) {
        /* If current stored entry is both:
            1. Useable (ignoring ACPI reclaimable)
            2. Longer length than `ret`
           then set ret to the current entry */
        if (ret.length < e820_entries[i].length &&
            e820_entries[i].type == E820_USEABLE) {
            ret = e820_entries[i];
        }
    }
    return ret;
}

size_t get_total_e820_size(){
    size_t ret = 0;
    for(count_t i = 0; i < E820_MAX; i++) ret += e820_entries[i].length;
    return ret;
}