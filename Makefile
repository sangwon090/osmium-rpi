.PHONY: qemu clean install

RPI?=RPI4
export RPI

kernel8.img:
	$(MAKE) -C src/kernel
	cp src/kernel/kernel8.img kernel8.img

qemu: kernel8.img
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio

install: kernel8.img
	rm -f /Volumes/boot/kernel8.img
	cp kernel8.img /Volumes/boot/kernel8.img
	diskutil unmount /Volumes/boot/

clean:
	$(MAKE) -C src/kernel clean
	rm -f kernel8.img