%include "gdt.asm"
%include "util/err/error.asm"
%include "util/err/error_codes.asm"
[bits 16]
; takes a callback function address in edx, 
; jumps there after prot mode was entered
enter_prot_mode:
    cli                     ; no ints please
    lgdt [gdt_descriptor]   ; load gdt table
    mov eax, cr0            ; can't write directly to cr0, 
                            ; instead move to gpr and then move back
    or eax, 0x1             ; set prot mode on
    mov cr0, eax            ; move eax back to cr0

    jmp CODE_SEG:prot_mode_early_init   ; far jmp to reload cs

[bits 32]
prot_mode_early_init:
    mov ax, DATA_SEG    ; set up segments
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000    ; set up stack
    mov esp, ebp

    jmp edx             ; jump to the callback provided