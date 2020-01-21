%define DISK_INT 0x13
%define DISK_READ_FUNC 0x2

%include "util/err/error_codes.asm"
%include "util/err/error.asm"
;read `dh` sectors from drive `dl` into ES:BX starting at sector ax
disk_read:
    pusha
    push dx ; this gets overwritten, save it to be popped

    mov cx, ax              ;cylinder
    mov ah, DISK_READ_FUNC  ;reading from disk
    mov al, dh              ;amount of sectors
    mov dh, 0x00            ;head 0

    int DISK_INT            ;carry bit is set upon error
    jc disk_error           ;jump to error if bios gave us an error

    pop dx                  ;restoring this
    cmp al, dh              ;check we read the right number of sectors
    jne sector_error        ;read incorrect amount of sectors

    popa
    ret

disk_error:
    mov ax, ERROR_DISK_READ
    mov bx, disk_error_string
    jmp error

sector_error:
    mov ax, ERROR_DISK_WRONG_SECTOR_CNT
    mov bx, sector_error_string
    jmp error

disk_error_string:
    db 'Error while reading disk', 0

sector_error_string:
    db 'Invalid sector count', 0