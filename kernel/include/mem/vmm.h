/*
 *  vmm.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _mem_vmm_h
#define _mem_vmm_h

#include <inttypes.h>
#include <mem/pmm.h>

typedef size_t *vmm_page_table;
vaddr_info_t vaddr_to_paddr(vaddr_t vaddr);

#endif /* _mem_vmm_h */