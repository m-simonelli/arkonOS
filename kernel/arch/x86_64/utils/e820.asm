extern call_real
%include "bootloader/i386/kern_info.asm"
section .data
%define e820_func_size e820_func_end - e820_func_start
e820_func_start:    incbin "kernel/arch/x86_64/utils/real/e820.bin"
e820_func_end:

section.text

[bits 64]
global do_e820_mem_map
; address to write e820 map to in rdi
do_e820_mem_map:
    ; store registers that get trashed
    push rcx
    push rsi
    push rax
    push rdx
    push rdi
    push rbx

    ; argument for function is rdi
    mov rbx, rdi

    ; call_real copies rcx bytes from rsi to 0x8000
    mov rsi, e820_func_start-kernel_vaddr
    mov rcx, e820_func_size
    call call_real

    ; restore trashed registers
    pop rbx
    pop rdi
    pop rdx
    pop rax
    pop rsi
    pop rcx

    ret