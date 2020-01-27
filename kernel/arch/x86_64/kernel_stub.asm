[bits 32]
section .kentry ; make sure the linker puts this first
[extern k_start]
_start:
    call k_start
    jmp $