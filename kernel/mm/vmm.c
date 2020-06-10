/*
 *  vmm.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <mm/vmm.h>
#include <string.h>

#define VADDR_FLAG_BITS (0x3FF)
#define VADDR_ADDR_BITS (~VADDR_FLAG_BITS)

vaddr_info_t vaddr_to_paddr(vaddr_t vaddr) {
    uint64_t pml4_entry, pdp_entry, pd_entry;
    vaddr_info_t ret = {0};
    ret.vaddr = vaddr;

    ret.pml4 = (paddr_t *)((uint64_t)&PML4 - KERN_VADDR);
    ret.pml4_idx = (vaddr >> 39) & 0x1FF;
    pml4_entry = ret.pml4[ret.pml4_idx];
    ret.pml4_attr = pml4_entry & PAGE_FLAG_BITS;

    ret.pdp = (paddr_t *)(pml4_entry & ~PAGE_FLAG_BITS);
    ret.pdp_idx = (vaddr >> 30) & 0x1FF;
    pdp_entry = ret.pdp[ret.pdp_idx];
    ret.pdp_attr = pdp_entry & PAGE_FLAG_BITS;

    ret.pd = (paddr_t *)(pdp_entry & ~PAGE_FLAG_BITS);
    ret.pd_idx = (vaddr >> 21) & 0x1FF;
    pd_entry = ret.pd[ret.pd_idx];
    ret.pd_attr = pd_entry & PAGE_FLAG_BITS;

    ret.pt = (paddr_t *)(pd_entry & ~PAGE_FLAG_BITS);
    ret.pt_idx = (vaddr >> 12) & 0x1FF;
    ret.paddr = ret.pt[ret.pt_idx];
    ret.pt_attr = ret.paddr & PAGE_FLAG_BITS;

    return ret;
}

void vmm_map(vaddr_t vaddr, paddr_t paddr, uint32_t flags,
             vmm_page_table pml4) {
    uint64_t p4idx = (vaddr >> 39) & 0x1FF;
    uint64_t p3idx = (vaddr >> 30) & 0x1FF;
    uint64_t p2idx = (vaddr >> 21) & 0x1FF;
    uint64_t p1idx = (vaddr >> 12) & 0x1FF;

    uint64_t *pml3 = NULL;
    uint64_t *pml2 = NULL;
    uint64_t *pml1 = NULL;

    if (pml4[p4idx] & PAGE_PRESENT) {
        pml3 = (uint64_t *)(pml4[p4idx] & PAGE_ADDR_MASK);
    } else {
        pml3 = (uint64_t *)pmm_alloc_block();
        pml4[p4idx] = (uint64_t)pml3 | PAGE_PRESENT | PAGE_WRITEABLE;
        bzero(((uint64_t)pml3 & PAGE_ADDR_MASK), PAGE_SIZE);
    }

    if (pml3[p3idx] & PAGE_PRESENT) {
        pml2 = (uint64_t *)(pml3[p3idx] & PAGE_ADDR_MASK);
    } else {
        pml2 = (uint64_t *)pmm_alloc_block();
        pml3[p3idx] = (uint64_t)pml2 | PAGE_PRESENT | PAGE_WRITEABLE;
        bzero(((uint64_t)pml2 & PAGE_ADDR_MASK), PAGE_SIZE);
    }

    if (pml2[p2idx] & PAGE_PRESENT) {
        pml1 = (uint64_t *)(pml2[p2idx] & PAGE_ADDR_MASK);
    } else {
        pml1 = (uint64_t *)pmm_alloc_block();
        pml2[p2idx] = (uint64_t)pml1 | PAGE_PRESENT | PAGE_WRITEABLE;
        bzero(((uint64_t)pml1 & PAGE_ADDR_MASK), PAGE_SIZE);
    }

    pml1[p1idx] = paddr | PAGE_PRESENT | PAGE_WRITEABLE | flags;
}