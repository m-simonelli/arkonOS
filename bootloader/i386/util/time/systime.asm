; systime.asm
; Copyright Marco Simonelli 2020
; You are free to redistribute/modify this code under the 
; terms of the GPL version 3 (see the file LICENSE)

[bits 16]
int_get_systime:
    xor ah, ah
    int 0x1A
    mov [stime.h], cx
    mov [stime.l], dx
    ret

[bits 32]
get_systime:
    xor eax, eax
    mov ax, [stime.h]
    shl eax, 16
    mov ax, [stime.l]
    ret

stime:
  .h:  dw 0
  .l:  dw 0