; kernel_stub.asm
; Copyright Marco Simonelli 2020
; You are free to redistribute/modify this code under the 
; terms of the GPL version 3 (see the file LICENSE)

[bits 32]
section .kentry ; make sure the linker puts this first
[extern k_start]
_start:
    call k_start
    jmp $