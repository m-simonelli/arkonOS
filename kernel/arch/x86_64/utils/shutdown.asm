; shutdown.asm
; Copyright Marco Simonelli 2020
; You are free to redistribute/modify this code under the 
; terms of the GPL version 3 (see the file LICENSE)

extern call_real
%include "bootloader/i386/kern_info.asm"
section .data
%define shutdown_func_size shutdown_func_end - shutdown_func_start
shutdown_func_start:    incbin "kernel/arch/x86_64/utils/real/shutdown.bin"
shutdown_func_end:

extern _panic

section .text
[bits 64]
global shutdown
shutdown:
    ; don't bother saving registers, this will never return
    ; call_real copies rcx bytes from rsi to 0x8000
    mov rsi, shutdown_func_start-kernel_vaddr
    mov rcx, shutdown_func_size
    call call_real
    ; never reached
    call _panic