







OUTPUT_FORMAT(binary)

SECTIONS {
    kernel_vaddr = 0xFFFFFFFFC0000000;
    . = 0x100000 + 0xFFFFFFFFC0000000;
    .kentry ALIGN(4K) : {
      *(.kentry)
    }
    .text ALIGN(4K) : {
      KEEP(*(.entry))
      *(.text*)
    }
    .rodata ALIGN(4K) : {
      *(.rodata*)
    }
    .data ALIGN(4K) : {
      *(.data*)
    }
    .bss ALIGN(4K) : {
      bss_begin = .;
      *(COMMON)
      *(.bss*)
      bss_end = .;
    }
    kern_end = .;
}
