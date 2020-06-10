/*
 *  vmm.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _mm_vmm_h
#define _mm_vmm_h

#include <inttypes.h>
#include <mm/pmm.h>

typedef uint64_t *vmm_page_table;

vaddr_info_t vaddr_to_paddr(vaddr_t vaddr);

/*!
    @function vmm_map
    Map 1 page of virtual address space to physical address space given flags

    @param vaddr
    The virtual address where the virtual page that will be mapped to physical
    address space begins

    @param paddr
    The physical address where vaddr should be mapped to

    @param flags
    Additional flags to be set in the PML1 (PAGE_PRESENT and PAGE_WRITEABLE
    are both default and compulsory)

    @param pml4
    The address of the PML4
!*/
void vmm_map(vaddr_t vaddr, paddr_t paddr, uint32_t flags, vmm_page_table pml4);

#endif /* _mm_vmm_h */