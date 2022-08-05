C_FILES   = $(wildcard *.c) $(wildcard */*.c)
ASM_FILES = $(wildcard *.S) $(wildcard */*.S)
OBJ_FILES = $(C_FILES:%.c=%.c.o) $(ASM_FILES:%.S=%.s.o)

TOOLCHAIN = /Applications/ARM/bin
GCC_FLAGS = -O2 -Wall -Wextra -ffreestanding -nostdinc -nostdlib -nostartfiles -mcpu=cortex-a53+nosimd -D${RPI} -I../../include/kernel
ASM_FLAGS =  -I../../include/kernel

.PHONY: clean
all: clean kernel8.img

%.s.o: %.S
	$(TOOLCHAIN)/aarch64-none-elf-gcc $(ASM_FLAGS) -c $< -o $@ -g -D__ASSEMBLER__

%.c.o: %.c
	$(TOOLCHAIN)/aarch64-none-elf-gcc $(GCC_FLAGS) -c $< -o $@ -g

kernel8.elf: $(OBJ_FILES)
	$(TOOLCHAIN)/aarch64-none-elf-ld -nostdlib $(OBJ_FILES) -T linker.ld -o kernel8.elf

kernel8.img: kernel8.elf
	$(TOOLCHAIN)/aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

kernel8.sym: kernel8.elf
	$(TOOLCHAIN)/aarch64-none-elf-objcopy --only-keep-debug kernel8.elf kernel8.sym

clean:
	rm -f *.o */*.o *.elf */*.elf *.img */*.img *.sym */*.sym