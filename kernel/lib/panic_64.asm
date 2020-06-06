;  panic_64.asm
;  Copyright Marco Simonelli 2020
;  You are free to redistribute/modify this code under the 
;  terms of the GPL version 3 (see the file LICENSE)

[bits 64]
global _panic_save_regs_to_p_regs

[extern p_regs]
section .text

; struct panic_dump {
;     /* Address of panic */
;     size_t ip;
; 
;     /* General purpose registers */
;     size_t ax;
;     size_t bx;
;     size_t cx;
;     size_t dx;
;     size_t si;
;     size_t di;
;     size_t sp;
;     size_t bp;
; 
; #ifdef KERN_TARGET_x86_64
;     /* These are only available on x86_64 */
;     size_t r8;
;     size_t r9;
;     size_t r10;
;     size_t r11;
;     size_t r12;
;     size_t r13;
;     size_t r14;
;     size_t r15;
; #endif /* KERN_TARGET_x86_64 */
; 
;     /* Useful control registers */
;     size_t cr2;
;     size_t cr3;
; };

_panic_save_regs_to_p_regs:
    ; size_t is guaranteed to be 8 bytes on x86_64
    ; general purpose registers
    mov [p_regs + 8], rax
    mov [p_regs + 16], rbx
    mov [p_regs + 24], rcx
    mov [p_regs + 32], rdx

    ; source and destination index
    mov [p_regs + 40], rsi
    mov [p_regs + 48], rdi

    ; more general purpose
    mov [p_regs + 72], r8
    mov [p_regs + 80], r9
    mov [p_regs + 88], r10
    mov [p_regs + 96], r11
    mov [p_regs + 104], r12
    mov [p_regs + 112], r13
    mov [p_regs + 120], r14
    mov [p_regs + 128], r15

    ; important control regs
    mov rax, cr2
    mov [p_regs + 136], rax
    mov rax, cr3
    mov [p_regs + 144], rax

    ; finally the address of panic
    pop rax
    ; but first, stack
    mov [p_regs + 56], rsp
    mov [p_regs + 64], rbp
    push rax
    mov [p_regs], rax
    ret