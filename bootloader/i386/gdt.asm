; gdt.asm
; Copyright Marco Simonelli 2020
; You are free to redistribute/modify this code under the 
; terms of the GPL version 3 (see the file LICENSE)

; standard gdt table
%ifndef _gdt_asm
%define _gdt_asm

gdt_start:
    dq 0x0  ; gdt starts with 8 bytes of 0

gdt_code:
    dw 0xffff       ; length of segment
    dw 0x0          ; segment base
    db 0x0          ; segment base (cont., upper 8 bits, 16-23)
    db 10011010b    ; flags
    db 11001111b    ; flags:4 + seg length upper 4 bits
    db 0x0          ; segment base (another upper 8 bits,24-31)

gdt_data:
    dw 0xffff       ; length of segment
    dw 0x0          ; segment base
    db 0x0          ; segment base (cont., upper 8 bits, 16-23)
    db 10010010b    ; flags
    db 11001111b    ; flags:4 + seg length upper 4 bits
    db 0x0          ; segment base (another upper 8 bits,24-31)

gdt_end:            ; for calculating size

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size
    dd gdt_start                ; start addr

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
%endif