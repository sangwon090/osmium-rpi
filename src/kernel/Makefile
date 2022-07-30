C_FILES   = $(wildcard *.c) $(wildcard */*.c)
ASM_FILES = $(wildcard *.S) $(wildcard */*.S)
OBJ_FILES = $(C_FILES:%.c=%_c.o) $(ASM_FILES:%.S=%_s.o)

TOOLCHAIN = /Applications/ARM/bin
GCC_FLAGS = -O2 -Wall -Wextra -ffreestanding -nostdinc -nostdlib -nostartfiles -mcpu=cortex-a53+nosimd -D${RPI} -I../../include/kernel
ASM_FLAGS =  -I../../include/kernel

.PHONY: clean
all: clean kernel8.img

%_s.o: %.S
	$(TOOLCHAIN)/aarch64-none-elf-gcc $(ASM_FLAGS) -c $< -o $@

%_c.o: %.c
	$(TOOLCHAIN)/aarch64-none-elf-gcc $(GCC_FLAGS) -c $< -o $@

kernel8.img: $(OBJ_FILES)
	$(TOOLCHAIN)/aarch64-none-elf-ld -nostdlib $(OBJ_FILES) -T linker.ld -o kernel8.elf
	$(TOOLCHAIN)/aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm -f *.o */*.o *.elf */*.elf *.img */*.img *.sym