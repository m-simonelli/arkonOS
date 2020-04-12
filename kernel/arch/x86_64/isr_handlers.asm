; isr.asm
; Copyright Marco Simonelli 2020
; You are free to redistribute/modify this code under the 
; terms of the GPL version 3 (see the file LICENSE)

%macro pusham 0
    cld
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popam 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

extern exception_handler
extern irq_handler

%macro isr_with_eno 1
    pusham

    mov rdi, %1
    mov rsi, rsp
    
    cld
    call exception_handler
    
    popam

    iretq
%endmacro

%macro irq_with_eno 1
    cli
   
    pusham
 
    mov rdi, %1
    mov rsi, rsp
   
    cld
    call irq_handler
    
    mov al, 0x20
%if %1 >= 40
    out 0xA0, al
%endif
    out 0x20, al

    popam
 
    iretq
%endmacro

; ISRs
global isr_div0
global isr_debug
global isr_nmi
global isr_breakpoint
global isr_overflow
global isr_bound_range
global isr_inv_opcode
global isr_no_dev
global isr_double_fault
global isr_cp_overrun
global isr_inv_tss
global isr_no_segment
global isr_ss_fault
global isr_gpf
global isr_page_fault
global isr_x87_fp
global isr_alignment_check
global isr_machine_check
global isr_simd_fp
global isr_virt
global isr_security

; IRQs
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

section .text

isr_div0:
    isr_with_eno 0x0
isr_debug:
    isr_with_eno 0x1
isr_nmi:
    isr_with_eno 0x2
isr_breakpoint:
    isr_with_eno 0x3
isr_overflow:
    isr_with_eno 0x4
isr_bound_range:
    isr_with_eno 0x5
isr_inv_opcode:
    isr_with_eno 0x6
isr_no_dev:
    isr_with_eno 0x7
isr_double_fault:
    isr_with_eno 0x8
isr_cp_overrun:
    isr_with_eno 0x9
isr_inv_tss:
    isr_with_eno 0xa
isr_no_segment:
    isr_with_eno 0xb
isr_ss_fault:
    isr_with_eno 0xc
isr_gpf:
    isr_with_eno 0xd
isr_page_fault:
    isr_with_eno 0xe
isr_x87_fp:
    isr_with_eno 0x10
isr_alignment_check:
    isr_with_eno 0x11
isr_machine_check:
    isr_with_eno 0x12
isr_simd_fp:
    isr_with_eno 0x13
isr_virt:
    isr_with_eno 0x14
isr_security:
    isr_with_eno 0x1e

irq0:
    irq_with_eno 32
irq1:
    irq_with_eno 33
irq2:
    irq_with_eno 34
irq3:
    irq_with_eno 35
irq4:
    irq_with_eno 36
irq5:
    irq_with_eno 37
irq6:
    irq_with_eno 38
irq7:
    irq_with_eno 39
irq8:
    irq_with_eno 40
irq9:
    irq_with_eno 41
irq10:
    irq_with_eno 42
irq11:
    irq_with_eno 43
irq12:
    irq_with_eno 44
irq13:
    irq_with_eno 45
irq14:
    irq_with_eno 46
irq15:
    irq_with_eno 47

scratch_reg: dq 0