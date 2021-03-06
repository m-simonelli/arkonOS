/*
 *  e820.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _mm_e820_h
#define _mm_e820_h

#include <inttypes.h>

#ifndef _E820_MAX_DEFINED_
#define _E820_MAX_DEFINED_
#define E820_MAX 128
#endif /* _E820_MAX_DEFINED_ */

/* These values are the "type" field of an e820 entry */
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

typedef struct e820_entry {
    uint64_t base;
    uint64_t length;
    uint64_t end;
    enum e820_region_type type;
} e820_entry_t;

/*!
    @function init_e820
    Initialize e820 resources, storing all entries in an array, finding
    most suitable region for the pmm etc

    @param e820_addr
    The address at which the e820 entries start, the first uint32_t at this
    address should be the amount of elements in the list
!*/
void init_e820(void *e820_addr);

/*!
    @function get_total_e820_size
    Get the total size of memory as reported by e820

    @return
    The size (in bytes) of memory as reported by e820
!*/
size_t get_total_e820_size();

extern void *e820_map_addr;
extern e820_entry_t e820_entries[E820_MAX];
extern size_t total_e820_size;
extern size_t used_e820_entries;

#endif /* _mm_e820_h */