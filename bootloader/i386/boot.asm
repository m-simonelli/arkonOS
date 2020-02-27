; boot.asm
; Copyright Marco Simonelli 2020
; You are free to redistribute/modify this code under the 
; terms of the GPL version 3 (see the file LICENSE)

[org 0x7c00]
%include "kern_info.asm"
; before all includes so that this runs first
start:
    call clear_screen
    mov bx, boot_string
    call print_unformatted_string

    mov bp, 0x8000 ; setup stack
    mov sp, bp

    call load_kernel

    ; kernel is 32 bit, now need to switch to protected mode
    mov edx, prot_mode_init ; callback for after prot mode was entered
    jmp enter_prot_mode     ; this will return to the function in edx, which 
                            ; is prot_mode_init

    jmp halt    ; this should never run

[bits 16]
; read kernel 4kb at a time and write starting at KERN_LOAD_ADDR
load_kernel:
    pusha
    push ds
    push es
    ; ensure ds and es are 0
    push 0
    pop ds
    push 0
    pop es

    ;memcpy setup
    mov edi, KERN_LOAD_ADDR

    xor eax, eax
    mov edx, 1
.read_loop:
    ; set ds:si to where the disk address packet is
    mov esi, da_packet
    call disk_read
    ; this will break if blknum doesn't fit in a dword, i'll assume that never
    ; happens 
    mov eax, [da_packet.blknum]
    inc eax
    mov [da_packet.blknum], eax
    ; memcpy to KERN_LOAD_ADDR
    ; ds and es are guaranteed to be 0
    mov esi, 0x20000    ; temporary buffer address, the same as 0x2000:0x0000
    mov ecx, 512     ; 512 bytes, 1 sector
    o32 a32 rep movsb
    ; increase sectors read count
    inc edx
    cmp edx, KERN_SECTOR_COUNT
    jb .read_loop
    pop es
    pop ds
    popa
    ret

%include "util/print/print.asm"
%include "util/err/error.asm"
%include "util/disk.asm"
%include "util/pm/protmode.asm"
%include "util/a20/a20.asm"

boot_string:
    db 'Booting', NEW_LINE, 0
kernel_load_string:
    db 'Loading kernel...', NEW_LINE, 0
newline:
    db NEW_LINE, 0

BOOT_DRIVE db 0x80
[bits 32]
prot_mode_init:
    jmp KERN_LOAD_ADDR ; transfer ctrl to kernel
    jmp halt

times 510-($-$$) db 0
dw 0xaa55