#ifndef _mem_e820_h
#define _mem_e820_h

#include <inttypes.h>

#ifndef _E820_MAX_DEFINED_
#define _E820_MAX_DEFINED_
#define E820_MAX 128
#endif /* _E820_MAX_DEFINED_ */

enum e820_region_type {
    E820_UNKNOWN,          /* Unknown memory type, treat as unuseable */
    E820_USEABLE,          /* Normal (useable) RAM */
    E820_RESERVED,         /* Reserved, unuseable */
    E820_ACPI_RECLAIMABLE, /* Useable if finished using the ACPI
                               tables that are stored there */
    E820_ACPI_NVS,         /* ACPI non volatile, unuseable */
    E820_BAD_MEM,          /* Bad memory unuseable */
    E820_PRAM_OLD,         /*
                               Non standard way for ADR or NVDIMM regions
                               that persist over reboot, on old systems
                               this is type 6, on new systems it is type 12.
                               Treat as unuseable.
                           */
    E820_PMEM,             /* Persistent mem, treat as unuseable */
    E820_PRAM = 12         /* Persistent RAM, treat as unuseable */
};

struct e820_entry {
    uint64_t base;
    uint64_t length;
    uint64_t end;
    enum e820_region_type type;
};

typedef struct e820_entry e820_entry_t;

void init_e820(mem_ptr_t e820_addr);
e820_entry_t get_longest_contiguous_region(uint8_t skip);

extern mem_ptr_t e820_map_addr;

#endif /* _mem_e820_h */