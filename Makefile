.PHONY: qemu debug install clean

RPI?=RPI4
export RPI

kernel8.img:
	$(MAKE) kernel8.img -C src/kernel
	cp src/kernel/kernel8.img kernel8.img

kernel8.sym:
	$(MAKE) kernel8.sym -C src/kernel
	cp src/kernel/kernel8.sym kernel8.sym

qemu: kernel8.img
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio

debug: kernel8.img kernel8.sym
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio -s -S

install: kernel8.img
	rm -f /Volumes/boot/kernel8.img
	cp kernel8.img /Volumes/boot/kernel8.img
	diskutil unmount /Volumes/boot/

clean:
	$(MAKE) -C src/kernel clean
	rm -f kernel8.img kernel8.sym