[org 0x7c00]
%include "kern_info.asm"
; before all includes so that this runs first
start:
    call clear_screen
    mov bx, boot_string
    call print_unformatted_string

    mov bp, 0x8000 ;setup stack
    mov sp, bp

    call load_kernel

    ;kernel is 32 bit, now need to switch to protected mode
    mov edx, prot_mode_init ;callback for after prot mode was entered
    jmp enter_prot_mode     ;this will return to the function in edx, which 
                            ;is prot_mode_init

    jmp halt    ;this should never run

[bits 16]
; read kernel 4kb at a time and write starting at KERN_LOAD_ADDR
load_kernel:
    pusha
    push 0x2000 ; set up segment registers
    pop es
    push 0
    pop ds
    mov ax, 0x2 ; starting at disk sector 2, right after boot sector
    xor ebx, ebx ; count how many bytes we've read
.disk_loop:
    push ebx ; store read bytes count
    mov dh, 8 ; read 8 sectors = 4kb
    mov dl, [BOOT_DRIVE] ; drive 0
    xor bx, bx ; writing to es:bx, which is 0x2000:0 => 0x20000
    call disk_read

; now we memcpy the 4kb we read to the load phys addr of the kernel
    ;restore read bytes count
    pop ebx

    ;movsb moves a byte from DS:SI to ES:DI
    ;es is 0x2000, so we subtract 0x2000*0x10 from the load addr and write to 
    ;0x2000:(KERN_LOAD_ADDR-0x20000)
    mov edi, KERN_LOAD_ADDR - 0x20000
    ;add the amount of bytes we've read so we are writing right after 
    ;where the previous write finished
    add edi, ebx
    ;reading from ds:esi => 0:0x20000 which is where the 
    ;4kb we just read is stored at
    mov esi, 0x20000
    mov ecx, 4096
    ;32 bit address modifier for nasm
    a32 rep movsb
    ;we have now read another 4096 bytes, increase the read byte count
    add ebx, 4096
    ;increase the current sector by 8 sectors
    add ax, 0x8
    ;check if we've read at least as many sectors as the kernel takes
    cmp ax, KERN_SECTOR_COUNT
    ;if below, still need to read more
    jb .disk_loop
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

BOOT_DRIVE db 0
[bits 32]
prot_mode_init:
    jmp KERN_LOAD_ADDR ;transfer ctrl to kernel
    jmp halt

times 510-($-$$) db 0
dw 0xaa55