/*
 *  pmm.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <k_log.h>
#include <mm/e820.h>
#include <mm/pmm.h>
#include <panic.h>
#include <string.h>
#include <conf.h>

/* Handy macro to calculate bitmap size */
#define BITMAP_SIZE(mem_size) (mem_size / PAGE_SIZE / 8)
#define BYTES_TO_PAGES(bytes) (((uint64_t)bytes + (PAGE_SIZE - 1)) / PAGE_SIZE)

size_t bitmap_size;
uint_t bitmap_vaddr;
uint_t bitmap_paddr;

#define bitmap_sbit(map, idx) (*((uint8_t *)map + (idx / 8)) |= (1 << idx % 8))
#define bitmap_cbit(map, idx) (*((uint8_t *)map + (idx / 8)) &= ~(1 << idx % 8))
#define bitmap_rbit(map, idx) ((*((uint8_t *)map + (idx / 8)) >> (idx % 8)) & 1)

#include <conf.h>

#define round_up(val, denom)                                     \
    ((((uint64_t)val + (uint64_t)denom - 1) / (uint64_t)denom) * \
     (uint64_t)denom)

#define round_down(val, denom) \
    ((((uint64_t)val) / (uint64_t)denom) * (uint64_t)denom)

void pmm_free_block(uint_t blk) { bitmap_cbit(bitmap_vaddr, blk); }

uint_t first_free_block() {
    for (size_t i = 0; i < bitmap_size * 8; i++)
        if (!bitmap_rbit(bitmap_vaddr, i)) return i;
    panic("PMM: Memory full!");
}

void *pmm_alloc_block() {
    uint_t blk = first_free_block();
    bitmap_sbit(bitmap_vaddr, blk);
    return (void *)(blk * PAGE_SIZE);
}

void init_pmm(size_t mem_size) {
    bitmap_size = BITMAP_SIZE(mem_size);
#if KERN_VERBOSITY >= 2
    k_log("Initializing PMM\n");

    k_log("mem_size: %#08llx\n", mem_size);
    k_log("bitmap size: %#08llx\n", bitmap_size);
    k_log("kern_end: %p\n", kern_end);

    k_log("Setting up pages for PMM bitmap\n");
#endif
    bitmap_vaddr = round_up(kern_end, PD_HUGE_PAGE_SIZE);
    bitmap_paddr = round_up((size_t)kern_end - KERN_VADDR, PAGE_SIZE);

#if KERN_VERBOSITY >= 2
    k_log("PMM bitmap will be located at v:%#0*llx p:%#0*llx\n", 16,
          bitmap_vaddr, 16, bitmap_paddr);
#endif
    /* Write the phys address of the bitmap to the page tables (PML1) */
    uint_t pmm_pt_cur_addr = bitmap_paddr | 0x3;
    for (uint16_t i = bitmap_paddr / PAGE_SIZE;
         i < BYTES_TO_PAGES(bitmap_size) + bitmap_paddr / PAGE_SIZE; i++) {
        PT[i] = pmm_pt_cur_addr;
        pmm_pt_cur_addr += 0x1000;
    }

    /* Write the address of PT to the second entry of the page directory */
    PD[1] = ((uint64_t)PT - KERN_VADDR) | 0x3;

    /* Write the physical address of the bitmap to the page table */
    PT[(bitmap_vaddr >> 12) & 0x1FF] = bitmap_paddr | 0x3;

    /* Ensure the bitmap is zeroed out */
    bzero(bitmap_vaddr, bitmap_size);

    /* Mark all pages up to the end of the bitmap as used.
     * Everything below 0x100000 is unuseable, and 0x100000-bitmap_start is
     * the kernel */
    for (size_t i = 0;
         i < round_up(bitmap_paddr + bitmap_size, PAGE_SIZE) / PAGE_SIZE; i++) {
        pmm_alloc_block();
    }

    /* Mark all unusable pages reported by e820 */
    for (size_t i = 0; i < used_e820_entries; i++) {
        if (e820_entries[i].type != E820_USEABLE) {
            /* Only handle when the type is not
             * useable (ignore ACPI reclaimable) */
            size_t page_aligned_start =
                round_down(e820_entries[i].base, PAGE_SIZE);
            size_t page_aligned_end = round_up(e820_entries[i].end, PAGE_SIZE);
            size_t start_page = page_aligned_start / PAGE_SIZE;

            size_t section_page_count =
                round_up(page_aligned_end - page_aligned_start, PAGE_SIZE) /
                PAGE_SIZE;
            /* Mark all the pages used by this section as used */
            for (size_t j = 0; j < section_page_count; j++) {
                bitmap_sbit(bitmap_vaddr, (start_page + j));
            }
        }
    }
}