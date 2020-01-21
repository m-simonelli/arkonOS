%include "bootloader/i386/kern_info.asm"
global k_start
[bits 32]
[extern kmain]
[extern bss_begin]
[extern bss_end]
;absolute addresses in this file before page tables are loaded have to have 
;kernel_vaddr subtracted from them due to the linker basing everything from 
;kernel_vaddr

section .text
k_start:
    ;null the bss segment, where we have the page tables
    call null_bss
    ;load our 64bit gdt
    lgdt [gdt64.gdt_descriptor-kernel_vaddr]
    ;initialize the page tables by identity mapping first 2mb and mapping 
    ;kernel_vaddr to 0x0
    call page_table_init
    ;jump to 64bit code and reload cs
    jmp 0x8:(goto_kmain-kernel_vaddr)

section .rodata
gdt64:
.gdt_start:
.gdt_null:
    dq 0

; 64 bit mode kernel
.kernel_code_64:
    dq (1<<43) | (1<<44) | (1<<47) | (1<<53) ; code segment

.kernel_data:
    dq (1<<41) | (1<<44) | (1<<47) ; data segment


.tss:
    dq 0
    dq 0

.gdt_end:
.gdt_descriptor:
    dw .gdt_end - .gdt_start - 1  ;size
    dd .gdt_start-kernel_vaddr                ;start addr

section .text
;because bss_begin and bss_end are defined in kernel.ld, we can't do this 
;in the bootloader
null_bss:
    ;stosb writes a byte from ax to the address ES:EDI

    ;store and set es to 0
    ;o16 modifiers tell nasm to push 2 bytes rather than 4 bytes due to 
    ;code being 32 bit
    o16 push es
    o16 push 0
    o16 pop es

    ;null the bss segment
    ;writing to 0:bss_begin
    mov edi, bss_begin-kernel_vaddr
    ;writing bss_end-bss_begin bytes
    mov ecx, bss_end-kernel_vaddr
    sub ecx, bss_begin-kernel_vaddr
    ;writing 0's
    xor eax, eax
    ;repeat ecx times `stosb`
    rep stosb
    ;restore es
    o16 pop es
    ret

page_table_init:
    ;set first entry of PML4 to point to PDP
    mov eax, PDP-kernel_vaddr
    or ax, 0b11
    mov [PML4-kernel_vaddr], eax
    
    ;set last entry of PML4 to point to KPDP
    mov eax, KPDP-kernel_vaddr
    or ax, 0b11
    mov [(PML4-kernel_vaddr) + 511*8], eax
    
    ;set first entry of PDP to point to PD
    mov eax, PD-kernel_vaddr
    or ax, 0b11
    mov [PDP-kernel_vaddr], eax

    ;set last entry of KPDP to point to PD
    mov [(KPDP-kernel_vaddr) + 511*8], eax

    ;set first entry of PD to point to PT
    mov eax, PT-kernel_vaddr
    or ax, 0b11
    mov [PD-kernel_vaddr], eax

    ;identity map PT
    ;all entries have to have 0x03 at the end for present+writeable
    mov eax, 0x03
    mov edi, PT-kernel_vaddr
    ;loop counter
    ;every entry covers 4kb, 4x512 = 2048 = 2MB
    mov cx, 512 ;only map first 2MB
  .buildpt:
    ;write eax to the current entry
    mov [edi], eax
    ;increase eax by "4kb" (4096) so that next entry is also identity mapped
    add eax, 0x1000
    ;increase current entry pointer
    add edi, 8
    ;decrement ecx, compare to 0, and loop if not equal
    loop .buildpt

    ;cr3 has to point to the Page-Map Level-4 Table Base Address, 
    ;which has to be 4kb aligned
    mov eax, PML4-kernel_vaddr
    mov cr3, eax

    ;enable PAE and PGE
    mov eax, cr4
    or eax, (1 << 5) | (1 << 7)
    mov cr4, eax

    ;set Long Mode Enable bit of the EFER msr
    mov ecx, 0xc0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ;enable paging
    mov eax, cr0
    or eax, 1 | (1 << 31)
    mov cr0, eax

    ret

[bits 64]
goto_kmain:
    ;set up segments for data
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ;can't directly call addresses >2GiB from rip
    ;move address into register and call the register
    mov rax, kmain
    call rax
    ;function is a c function, hence it will return
    ;hang here instead of letting execution continue
    jmp $

section .bss
pagemap equ PML4
PML4:   resb 4096
PDP:    resb 4096
KPDP:   resb 4096
PD:     resb 4096
PT:     resb 4096
stack_top equ PML4