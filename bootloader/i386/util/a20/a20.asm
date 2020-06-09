; a20.asm
; Copyright Marco Simonelli 2020
; You are free to redistribute/modify this code under the 
; terms of the GPL version 3 (see the file LICENSE)

; We **could** try and have more A20 methods here, but these should work on
; 99% of modern systems so long as Fast A20 is present (even if it isn't,
; we have the BIOS method to fall back on.)
[bits 16]
enable_a20:
    mov     ax,2403h
    int     15h
    jb      a20_ns
    cmp     ah,0
    jnz     a20_ns
    
    mov     ax,2402h
    int     15h
    jb      a20_bios_failed
    cmp     ah,0
    jnz     a20_bios_failed
    
    cmp     al,1
    jz      a20_activated
    
    mov     ax,2401h
    int     15h
    jb      a20_bios_failed
    cmp     ah,0
    jnz     a20_bios_failed

a20_activated:
    ret

a20_bios_failed:
    ; enabling the a20 w BIOS failed, try fast a20
    in al, 0x92
    test al, 2
    jnz a20_activated
    or al, 2
    and al, 0xfe ; apparently some machines break if you write to bit 0
    out 0x92, al
    jmp a20_activated

a20_ns:
    cli
    hlt