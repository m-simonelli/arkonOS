; call_real.asm
; Copyright Marco Simonelli 2020
; You are free to redistribute/modify this code under the 
; terms of the GPL version 3 (see the file LICENSE)

; assume long mode
global call_real
%include "bootloader/i386/kern_info.asm"

section .data
%define real_init_func_size real_init_func_end - real_init_func_start
real_init_func_start: incbin "kernel/arch/x86_64/utils/real/real_init.bin"
real_init_func_end:

section .text
[bits 64]
; RSI = real function address
; RCX = real function size in bytes
call_real:
    ; function to call at 0x8000
    mov rdi, 0x8000
    rep movsb

    ; real mode init function at 0x1000
    mov rsi, real_init_func_start-kernel_vaddr
    mov rcx, real_init_func_size
    mov rdi, 0x1000
    rep movsb

    ; can't directly call address
    mov rax, 0x1000
    call rax
    ret