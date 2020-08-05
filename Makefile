
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
	cat $(BOOTSECT_BIN) $(KERNEL_BIN) > $(DISK_IMG)

run:
	qemu-system-i386 -fda $(DISK_IMG)

debug:
	qemu-system-i386 -s -fda $(DISK_IMG) -S &
	sleep 0.5s
	i386-elf-gdb

clean: 
	rm -rf build/
	rm -f $(DISK_IMG)

