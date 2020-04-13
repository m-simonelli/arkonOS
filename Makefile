#  Makefile
#  Copyright Marco Simonelli 2020
#  You are free to redistribute/modify this code under the 
#  terms of the GPL version 3 (see the file LICENSE)

VERSION=0.2.1-alpha

#leave these as is, support for other platforms is planned
PLATFORM=i386
KERN_PLATFORM=x86_64

#assembler
#ideally nasm or some nasm compatible assembler
AS=nasm

#compiler
CC=gcc

#linker
LD=ld

#objcopy
OBJCOPY=objcopy

#debugger
GDB=gdb
LLDB=lldb

#prefix for your cross compiler toolchain
TOOL_PREFIX=i386-elf-

#prefix for kernel compiler toolchain
KERN_TOOL_PREFIX=$(KERN_PLATFORM)-elf-

#qemu options
#Enable serial port redirection to device QEMU_REDIRECT_SERIAL_DEVICE
#if QEMU_REDIRECT_DEBUGCON is 1, then it takes presendence over this option
#as only one can be enabled
QEMU_REDIRECT_SERIAL=1
QEMU_REDIRECT_SERIAL_DEVICE=stdio

#Enable debug console redirection to device QEMU_REDIRECT_DEBUGCON_DEVICE
QEMU_REDIRECT_DEBUGCON=1
QEMU_REDIRECT_DEBUGCON_DEVICE=stdio

#extra qemu options
QEMU_OPTIONS=-m 512M

#set this to where you want the kernel to be initially loaded
#k_start (kernel/arch/PLATFORM/start.asm) will be at this physical 
#address after kmain() is hit, the kernel will relocate itself
KERN_LOAD_ADDR=0x100000

#enable this to copy all vga and other output to serial
KERN_LOG_TO_SERIAL=1

#enable qemu specific features (i.e. port 0xe9 output)
#1 = enabled, else not enabled
KERN_TARGET_QEMU=1

#Detailed options, these all require KERN_TARGET_QEMU=1
#Log all serial output to qemu's port 0xE9
#Use with LOG_TO_SERIAL (defined in drivers/serial/serial.h) to log
#everything to port 0xE9
KERN_QEMU_DEBUG_PORT_ENABLED=1

#virtual address where the kernel will be loaded
#please god please don't change this
#if you do you have to manually change page tables init
#to reflect the changes
KERN_VIRT_ADDR=0xFFFFFFFFC0000000

KERN_SECTOR_COUNT=wc -c < kernel.bin | awk '{printf("%.0f\n", ($$1+511)/512)}'

KERN_INCLUDE_DIR=kernel/include
KERN_HEADERS=		$(KERN_INCLUDE_DIR)/drivers/io/ports.h		\
					$(KERN_INCLUDE_DIR)/drivers/vga/vga.h		\
					$(KERN_INCLUDE_DIR)/drivers/io/kb.h			\
					$(KERN_INCLUDE_DIR)/drivers/serial/uart.h	\
					$(KERN_INCLUDE_DIR)/drivers/serial/serial.h	\
					$(KERN_INCLUDE_DIR)/libc/printf.h			\
					$(KERN_INCLUDE_DIR)/libc/stdarg.h			\
					$(KERN_INCLUDE_DIR)/inttypes.h				\
					$(KERN_INCLUDE_DIR)/limits.h				\
					$(KERN_INCLUDE_DIR)/mem/memcpy.h			\
					$(KERN_INCLUDE_DIR)/mem/memswp.h			\
					$(KERN_INCLUDE_DIR)/mem/memset.h			\
					$(KERN_INCLUDE_DIR)/mem/pmm.h				\
					$(KERN_INCLUDE_DIR)/mem/vmm.h				\
					$(KERN_INCLUDE_DIR)/mem/e820.h				\
					$(KERN_INCLUDE_DIR)/mem/strlen.h			\
					$(KERN_INCLUDE_DIR)/mem/string.h			\
					$(KERN_INCLUDE_DIR)/util/ascii_tools.h		\
					$(KERN_INCLUDE_DIR)/panic.h					\
					$(KERN_INCLUDE_DIR)/k_log.h					\
					$(KERN_INCLUDE_DIR)/kcmd.h					\
					$(KERN_INCLUDE_DIR)/conf.h					\
					$(KERN_INCLUDE_DIR)/arch/x86_64/8259_pic.h	\
					$(KERN_INCLUDE_DIR)/arch/x86_64/exception.h	\
					$(KERN_INCLUDE_DIR)/arch/x86_64/idt.h		\
					$(KERN_INCLUDE_DIR)/arch/x86_64/isr.h		\
					$(KERN_INCLUDE_DIR)/arch/x86_64/irq.h

KERN_SOURCE_DIR=kernel
KERN_C_SOURCES=		$(KERN_SOURCE_DIR)/init.c					\
					$(KERN_SOURCE_DIR)/kcmd.c					\
					$(KERN_SOURCE_DIR)/drivers/io/ports.c		\
					$(KERN_SOURCE_DIR)/drivers/io/kb.c			\
					$(KERN_SOURCE_DIR)/drivers/vga/vga.c		\
					$(KERN_SOURCE_DIR)/drivers/serial/uart.c	\
					$(KERN_SOURCE_DIR)/drivers/serial/serial.c	\
					$(KERN_SOURCE_DIR)/libc/printf.c			\
					$(KERN_SOURCE_DIR)/mem/memcpy.c				\
					$(KERN_SOURCE_DIR)/mem/memswp.c				\
					$(KERN_SOURCE_DIR)/mem/memset.c				\
					$(KERN_SOURCE_DIR)/mem/string.c				\
					$(KERN_SOURCE_DIR)/mem/pmm.c				\
					$(KERN_SOURCE_DIR)/mem/vmm.c				\
					$(KERN_SOURCE_DIR)/mem/e820.c				\
					$(KERN_SOURCE_DIR)/mem/strlen.c				\
					$(KERN_SOURCE_DIR)/util/ascii_tools.c		\
					$(KERN_SOURCE_DIR)/util/panic.c				\
					$(KERN_SOURCE_DIR)/util/dbg_log.c			\
					$(KERN_ARCH_DIR)/idt.c						\
					$(KERN_ARCH_DIR)/isr.c						\
					$(KERN_ARCH_DIR)/irq.c						\
					$(KERN_ARCH_DIR)/exception.c				\
					$(KERN_ARCH_DIR)/8259_pic.c

KERN_ARCH_DIR=kernel/arch/$(KERN_PLATFORM)
KERN_ASM_SOURCES= 	$(KERN_ARCH_DIR)/start.asm					\
					$(KERN_ARCH_DIR)/isr_handlers.asm			\
					$(KERN_ARCH_DIR)/kernel_stub.asm			\
					$(KERN_ARCH_DIR)/utils/e820.asm				\
					$(KERN_ARCH_DIR)/utils/call_real.asm		\
					$(KERN_SOURCE_DIR)/util/panic_64.asm

KERN_OBJECTS= 		${KERN_ASM_SOURCES:.asm=.o}					\
					${KERN_C_SOURCES:.c=.o}

KERN_REAL_SOURCES=	$(KERN_ARCH_DIR)/utils/real/e820.real		\
					$(KERN_ARCH_DIR)/utils/real/real_init.real

KERN_REAL_BINS=		${KERN_REAL_SOURCES:.real=.bin}

OS_IMAGE_NAME=arkon-$(VERSION)

CC_INCLUDE_DIRS=kernel/include/
CC_FLAGS=	-I$(CC_INCLUDE_DIRS) 	\
			-Wall				 	\
			-Wextra 				\
			-g 						\
			-O0 					\
			-mno-sse                \
			-mno-sse2               \
			-mno-sse3				\
			-mno-sse4				\
			-mno-mmx                \
			-mno-80387              \
			-mno-avx512f			\
			-mno-3dnow				\
			-mno-red-zone			\
			-mcmodel=kernel 		\

ifeq ("$(KERN_PLATFORM)","x86_64")
	KERN_ASM_PLATFORM=elf64
	KERN_LD_PLATFORM=elf64-x86-64
	CC_FLAGS+= -m64 -mcmodel=kernel
else
	KERN_ASM_PLATFORM=elf32
	KERN_LD_PLATFORM=elf32-i386
	CC_FLAGS+= -m32 -mcmodel=kernel
endif

ifeq ("$(QEMU_REDIRECT_DEBUGCON)","1")
	QEMU_OPTIONS+= -debugcon $(QEMU_REDIRECT_DEBUGCON_DEVICE)
else ifeq ("$(QEMU_REDIRECT_SERIAL)","1")
	QEMU_OPTIONS+= -serial $(QEMU_REDIRECT_SERIAL_DEVICE)
endif

define write_kern_config_h
	echo "#ifndef _kern_conf_h\n#define _kern_conf_h" > $(KERN_INCLUDE_DIR)/conf.h
	echo "#define KERN_TARGET_$(KERN_PLATFORM)" >> $(KERN_INCLUDE_DIR)/conf.h
	echo "#define KERN_TARGET_QEMU $(KERN_TARGET_QEMU)" >> $(KERN_INCLUDE_DIR)/conf.h
	echo "#define KERN_QEMU_DEBUG_PORT_ENABLED $(KERN_QEMU_DEBUG_PORT_ENABLED)" >> $(KERN_INCLUDE_DIR)/conf.h
	echo "#define LOG_TO_SERIAL $(KERN_LOG_TO_SERIAL)" >> $(KERN_INCLUDE_DIR)/conf.h
	echo "#define KERN_LOAD_ADDR $(KERN_LOAD_ADDR)" >> $(KERN_INCLUDE_DIR)/conf.h
	echo "#define KERN_VADDR $(KERN_VIRT_ADDR)" >> $(KERN_INCLUDE_DIR)/conf.h
	echo "#endif /* _kern_conf_h */" >> $(KERN_INCLUDE_DIR)/conf.h
endef

define write_kern_info_asm
	echo "%ifndef _boot_kern_info_asm" > bootloader/i386/kern_info.asm
	echo "%define _boot_kern_info_asm" >> bootloader/i386/kern_info.asm
	echo "%define KERN_LOAD_ADDR $(KERN_LOAD_ADDR)" >> bootloader/i386/kern_info.asm
	printf "%%define KERN_SECTOR_COUNT " >> bootloader/i386/kern_info.asm
	$(KERN_SECTOR_COUNT) >> bootloader/i386/kern_info.asm
	printf "%%define KERN_SEGMENT_ADDR " >> bootloader/i386/kern_info.asm
	printf $(KERN_LOAD_ADDR) | awk --non-decimal-data '{printf("0x%x\n", $$1/0x10)}' >> bootloader/i386/kern_info.asm
	printf "%%define KERN_SEGMENT_OFF " >> bootloader/i386/kern_info.asm
	printf $(KERN_LOAD_ADDR) | awk --non-decimal-data '{printf("%d", $$1 % 0x10)}' >> bootloader/i386/kern_info.asm
	echo "\n%define kernel_vaddr $(KERN_VIRT_ADDR)" >> bootloader/i386/kern_info.asm
	echo "%endif" >> bootloader/i386/kern_info.asm
endef

# arg 1 is input file, arg 2 is options for the preprocesser, arg 3 is 
# output file
define preprocess_file
	gcc -E -x c $(2) $(1) | grep -v '^#' >$(3)
endef

# arg 1 is out format
define gen_kern_linker
	echo "#define KERN_LOAD_ADDR $(KERN_LOAD_ADDR)" > kernel_inc.gen
	echo "#define KERN_OFORMAT $(1)" >> kernel_inc.gen
	echo "#define KERN_VADDR $(KERN_VIRT_ADDR)" >> kernel_inc.gen
	$(call preprocess_file,kernel.ld.gen,,kernel.ld)
endef

# arg 1 is target (elf, elf64, bin), arg 2 is include dir, arg 3 is input 
# file, arg 4 is output file
define assemble_as_with_include
	$(AS) -I $(2) $(3) -f $(1) -o $(4)
endef

# arg 1 is target, arg 2 is input file, arg 3 is output
define assemble_as 
	$(AS) $(2) -f $(1) -o $(3)
endef

# arg 1 is prefix, arg 2 is output file, arg 3 is arguments, 
# arg 4 is a variable name containing objects 
# arg 5 is a linker script
define link_with_prefix
	$(1)$(LD) -o $(2) $(3) -T$(5) $($(4))
endef

all: $(OS_IMAGE_NAME).bin

$(KERN_INCLUDE_DIR)/conf.h:
	$(call write_kern_config_h)

bootloader/i386/kern_info.asm:
	$(call write_kern_info_asm)

kernel/%.o: kernel/%.c ${KERN_HEADERS}
	$(KERN_TOOL_PREFIX)$(CC) $(CC_FLAGS) -ffreestanding -c $< -o $@ 

kernel/%.bin: kernel/%.real bootloader/i386/kern_info.asm
	$(call assemble_as,bin,$<,$@)
	
kernel/%.o: kernel/%.asm 
	$(call assemble_as,$(KERN_ASM_PLATFORM),$<,$@)

bootloader/%.o: %.asm
	$(call assemble_as_with_include,bin,bootloader/i386/,$<,$@)

boot.bin: bootloader/i386/boot.asm kernel.bin
	#this has to be written twice, as the first time is when compiling the 
	#kernel, which means the kernel sector count is unavailable the first time
	$(call write_kern_info_asm)
	$(call assemble_as_with_include,bin,bootloader/i386/,$<,$@)

kernel.bin: $(KERN_REAL_BINS) $(KERN_INCLUDE_DIR)/conf.h bootloader/i386/kern_info.asm ${KERN_OBJECTS}
	$(call gen_kern_linker,binary)
	$(call link_with_prefix,$(KERN_TOOL_PREFIX),$@,,KERN_OBJECTS,kernel.ld)
	
kernel.elf: ${KERN_OBJECTS}
	$(call gen_kern_linker,$(KERN_LD_PLATFORM))
	$(call link_with_prefix,$(KERN_TOOL_PREFIX),$@,,KERN_OBJECTS,kernel.ld)

$(OS_IMAGE_NAME).bin: boot.bin kernel.bin
	cat $^ > $@

clean:
	rm -rf *.bin *.o *.elf $(KERN_OBJECTS) bootloader/i386/*.o bootloader/i386/kern_info.asm kernel/arch/x86_64/*.o kernel.ld $(KERN_INCLUDE_DIR)/conf.h kernel_inc.gen $(KERN_REAL_BINS)

run: $(OS_IMAGE_NAME).bin
	qemu-system-x86_64 $(QEMU_OPTIONS) -hda $(OS_IMAGE_NAME).bin

debug-qemu: $(OS_IMAGE_NAME).bin
	qemu-system-x86_64 -d int -monitor stdio -no-shutdown -no-reboot -hda $(OS_IMAGE_NAME).bin

debug-gdb: $(OS_IMAGE_NAME).bin kernel.elf
	qemu-system-x86_64 $(QEMU_OPTIONS) -s -S -hda $(OS_IMAGE_NAME).bin &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

sourcedist: distclean
	find . -type f -name '*.DS_Store' -delete
	tar -czf $(OS_IMAGE_NAME).tar.gz --exclude *.tar.gz --exclude .git --exclude .vscode --exclude .gitignore --exclude *.DS_Store .
	sha256sum $(OS_IMAGE_NAME).tar.gz > $(OS_IMAGE_NAME).tar.gz.sha256sum

builddist: $(OS_IMAGE_NAME).bin
	find . -type f -name '*.DS_Store' -delete
	tar -czf $(OS_IMAGE_NAME).bin.tar.gz --exclude *.tar.gz --exclude .git --exclude .vscode --exclude .gitignore --exclude *.DS_Store $(OS_IMAGE_NAME).bin
	sha256sum $(OS_IMAGE_NAME).bin.tar.gz > $(OS_IMAGE_NAME).bin.tar.gz.sha256sum

dist: sourcedist builddist

distclean: clean
	rm -f $(OS_IMAGE_NAME).tar.gz $(OS_IMAGE_NAME).bin.tar.gz
	rm -f $(OS_IMAGE_NAME).tar.gz.sha256sum $(OS_IMAGE_NAME).bin.tar.gz.sha256sum

crun: clean run
cbuild: clean $(OS_IMAGE_NAME).bin
cdebug-gdb: clean debug-gdb
cdebug-qemu: clean debug-qemu
build: $(OS_IMAGE_NAME).bin