C_FILES   = $(wildcard *.c) $(wildcard */*.c)
OBJ_FILES = $(C_FILES:.c=.o)

TOOLCHAIN = /Applications/ARM/bin
GCC_FLAGS = -O2 -Wall -Wextra -ffreestanding -nostdinc -nostdlib -nostartfiles -D${RPI}

.PHONY: clean
all: clean kernel8.img

boot.o: boot.S
	$(TOOLCHAIN)/aarch64-none-elf-gcc $(GCC_FLAGS) -c boot.S -o boot.o

%.o: %.c
	$(TOOLCHAIN)/aarch64-none-elf-gcc $(GCC_FLAGS) -c $< -o $@

kernel8.img: boot.o $(OBJ_FILES)
	$(TOOLCHAIN)/aarch64-none-elf-ld -nostdlib boot.o $(OBJ_FILES) -T linker.ld -o kernel8.elf
	$(TOOLCHAIN)/aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	rm -f *.o */*.o *.elf */*.elf *.img */*.img *.sym