/*
 *  pmm.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _mem_pmm_h
#define _mem_pmm_h

#include <conf.h>
#include <inttypes.h>
#include <mem/e820.h>

/* How big pages are */
#define PAGE_SIZE 0x1000
#define PD_HUGE_PAGE_SIZE (PAGE_SIZE * 0x200)
#define BLOCK_SIZE PAGE_SIZE

/*!
    @function init_pmm
    Initalize the pmm

    @param mem_size
    The total size of RAM in bytes
!*/
void init_pmm(size_t mem_size);

#define PAGE_TABLE_ENTRY_COUNT 512

typedef size_t vaddr_t;
typedef size_t paddr_t;
typedef size_t page_table[PAGE_TABLE_ENTRY_COUNT];
typedef size_t *page_table_address_space;
typedef size_t *page_table_addr;

typedef void *vaddr_ptr_t;
typedef void *paddr_ptr_t;

typedef uint16_t mem_flags_t;
typedef uint16_t pt_idx_t;

typedef struct vaddr_info {
    vaddr_t vaddr;
    paddr_t paddr;
    paddr_t *pml4;
    paddr_t *pdp;
    paddr_t *pd;
    paddr_t *pt;
    pt_idx_t pml4_idx;
    pt_idx_t pml4_attr;
    pt_idx_t pdp_idx;
    pt_idx_t pdp_attr;
    pt_idx_t pd_idx;
    pt_idx_t pd_attr;
    pt_idx_t pt_idx;
    pt_idx_t pt_attr;
} vaddr_info_t;

#define PAGE_PRESENT (1 << 0)
#define PAGE_WRITEABLE (1 << 1)
#define PAGE_USER_ACCESS (1 << 2)
#define PAGE_WRITE_THROUGH (1 << 3)
#define PAGE_CACHE_DISABLE (1 << 4)
#define PAGE_ACCESSED (1 << 5)
#define PAGE_DIRTY (1 << 6)
#define PAGE_HUGE_PAGE (1 << 7)
#define PAGE_GLOBAL (1 << 8)

#define PAGE_FLAG_BITS                                                    \
    ((uint64_t)(PAGE_PRESENT | PAGE_WRITEABLE | PAGE_USER_ACCESS |        \
                PAGE_WRITE_THROUGH | PAGE_CACHE_DISABLE | PAGE_ACCESSED | \
                PAGE_DIRTY | PAGE_HUGE_PAGE | PAGE_GLOBAL |               \
                ((uint64_t)1 << 63)))

extern page_table kern_end;
extern page_table PML4;
extern page_table PDP;
extern page_table KPDP;
extern page_table PD;
extern page_table PMM_PD;
extern page_table PT;

enum page_sizes { PAGE_4KiB, PAGE_2MiB, PAGE_1GiB };

#endif /* _mem_pmm_h */