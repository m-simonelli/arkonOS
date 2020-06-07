/*
 *  e820.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <k_log.h>
#include <mm/e820.h>

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

void *e820_map_addr = NULL;
e820_entry_t e820_entries[E820_MAX] = {0};
size_t total_e820_size;
size_t used_e820_entries = 0;

void init_e820(void *e820_addr) {
    /* Iterate over all entries and log them to VGA */
    k_printf("%p\n", e820_addr);
    for (count_t i = 0; i < *((dword_t *)e820_addr); i++) {
        used_e820_entries++;
        /* Store each entry in an array */
        e820_entries[i] =
            (e820_entry_t){.base = *(uint64_t *)(e820_addr + 4 + (24 * i)),
                           .length = *(uint64_t *)(e820_addr + 12 + (24 * i)),
                           .end = *(uint64_t *)(e820_addr + 4 + (24 * i)) +
                                  *(uint64_t *)(e820_addr + 12 + (24 * i)),
                           .type = *(uint32_t *)(e820_addr + 20 + (24 * i))};
        /* Print the entry */
        k_log("start: %#08llx", e820_entries[i].base);
        k_printf("\tlength: %#08llx", e820_entries[i].length);
        k_printf("\ttype: %s", e820_region_type_strings[e820_entries[i].type]);
        k_printf("\n");
    }
    total_e820_size = get_total_e820_size();
    k_log("Total size: %#08llx\n", total_e820_size);
}

size_t get_total_e820_size() {
    size_t ret = 0;
    for (count_t i = 0; i < E820_MAX; i++) ret += e820_entries[i].length;
    return ret;
}