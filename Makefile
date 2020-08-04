
BUILD_DIR = build
BOOTSECT_BIN = $(BUILD_DIR)/boot/bootsect.bin
KERNEL_BIN = $(BUILD_DIR)/kernel/kernel.bin
KERNEL_ELF = $(BUILD_DIR)/kernel/kernel.elf
DISK_IMG = disk.img

all: bootdisk

.PHONY: run bootdisk bootsect kernel clean spotless debug drivers

bootsect: 
	make -C boot

drivers:
	make -C drivers

kernel: drivers
	make -C kernel

bootdisk: bootsect kernel
	dd if=/dev/zero of=$(DISK_IMG) bs=512 count=2880
	dd conv=notrunc if=$(BOOTSECT_BIN) of=$(DISK_IMG) bs=512 count=1 seek=0
	dd conv=notrunc if=$(KERNEL_BIN) of=$(DISK_IMG) bs=512 count=1 seek=1

run:
	qemu-system-i386 -fda $(DISK_IMG)

debug:
	qemu-system-i386 -s -fda $(DISK_IMG) -S &
	sleep 1s
	i386-elf-gdb

clean: 
	rm -rf build/

spotless: clean
	rm -f $(DISK_IMG)
