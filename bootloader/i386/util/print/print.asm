; print.asm
; Copyright Marco Simonelli 2020
; You are free to redistribute/modify this code under the 
; terms of the GPL version 3 (see the file LICENSE)

%ifndef _util_print_print_asm
%define _util_print_print_asm
%define LINE_FEED 0x0a
%define CAR_RETURN 0x0d
%define NEW_LINE LINE_FEED, CAR_RETURN

%define PRINT_INT 0x10

; takes a null-byte terminated string in bx, prints to tty
print_unformatted_string:
    pusha           ; store registers

; iterate until null byte
print_unformatted_string_loop:
    mov al, [bx]    ; get data at address that bx points to
    cmp al, 0       ; check if null byte
    je print_unformatted_string_done

    mov ah, 0x0e    ; write in tty mode
    int PRINT_INT   ; low byte of al contains the char

    inc bx          ; move char pointer 1 byte further
    jmp print_unformatted_string_loop

print_unformatted_string_done:
    popa            ; restore registers
    ret

; printing hex:
; if byte is 0x0 - 0x9, add 0x30
; if byte is 0xA - 0xF, add 0x37

; prints 1 byte from dx
print_hex_byte:
    pusha           ; store registers
    xor cx, cx      ; counter = 0

print_hex_byte_loop:
    cmp cx, 2       ; loop twice, one for each nibble of the byte
    je print_hex_byte_done

    mov al, dl      ; store the byte in a working registers
    and ax, 0x0f    ; get low nibble
    add al, 0x30    ; add 0x30
    cmp al, 0x39    ; if al is greater than 0x39, it's not 0-9
    jle print_hex_byte_write_to_output_string
    add al, 0x7     ; the nibble is 0xA-0xF, add 7 for a total of 0x37 added

print_hex_byte_write_to_output_string:
    mov bx, HEX_STR + 3 ; address of last character of output string
    sub bx, cx          ; subtract the index from the pointer
    mov [bx], al        ; write the character to the output string
    ror dl, 4           ; move to the next nibble

    add cx, 1           ; increase counter
    jmp print_hex_byte_loop ; go back to loop
print_hex_byte_done:
    mov bx, HEX_STR     ; print the string
    call print_unformatted_string
    popa                ; restore regsisters
    ret

clear_screen:       ; clear the screen
    pusha           ; store registers

    xor bx, bx      ; black background
    
    mov ax, 0x0700  ; scroll window
                    ; clear entire window
    mov bh, 0x07    ; white foreground
    mov cx, 0x00    ; sets top left as 0,0
    mov dx, 0x184f  ; 24 rows of chars
                    ; 79 cols of chars 
    int PRINT_INT

    popa            ; restore registers
    ret

HEX_STR:
    db '0x00', 0

%endif