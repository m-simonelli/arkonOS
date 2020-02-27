; disk.asm
; Copyright Marco Simonelli 2020
; You are free to redistribute/modify this code under the 
; terms of the GPL version 3 (see the file LICENSE)

%include "util/err/error_codes.asm"
%include "util/err/error.asm"
; read sectors from drive 0x80 (drive 0) with address packet ds:si
disk_read:
    pusha
    mov dl, 0x80
    mov cx, [da_packet.blkcnt]
    mov ah, 0x42
    int 0x13
    ; if cf is set, bios gave error
    jc disk_error
    ; check how many sectors were read
    mov dx, [da_packet.blkcnt]
    cmp cx, dx
    ; read incorrect amount of sectors
    jne sector_error
    popa
    ret

disk_error:
    ; int 13h sets ah to the error
    mov al, ah
    mov ah, 0
    mov bx, disk_error_string
    jmp error

sector_error:
    mov ax, ERROR_DISK_WRONG_SECTOR_CNT
    mov bx, sector_error_string
    jmp error

disk_error_string:
    db 'Disk read err', 0

sector_error_string:
    db 'Sector cnt err', 0

align 2
; values are preset to kernel load values to save bytes
da_packet:
.size:      db 0x10 ; size of packet (0x10 or 0x18 if using 64 bit extension)
            db 0    ; reserved (0)
.blkcnt:    dw 1 ; num of blocks to transfer
.bufoff:    dw 0x0000   ; transfer buffer segment offset
.bufseg:    dw 0x2000   ; transfer buffer segment
.blknum:    dd 1    ; starting block number
.blknumhi:  dd 0