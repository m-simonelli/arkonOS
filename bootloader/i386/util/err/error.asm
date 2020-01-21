
%ifndef _error_asm
%define _error_asm

%include "util/print/print.asm"
; error code in ax, error string pointed by bx
error:
    push ax
    ;print error string
    call print_unformatted_string
    ;print open bracket
    mov ax, 0x0e28
    int 0x10
    
    ;print the error code
    pop dx
    call print_hex_byte

    ;print close bracket
    mov ax, 0x0e29
    int 0x10

    ;halt the cpu
    jmp halt_loop
;halt and catch fire
halt:
    mov bx, halt_str
    call error
halt_loop:
    cli ;no interrupts
    hlt ;and halt

halt_str:
    db 'halting', 0
%endif