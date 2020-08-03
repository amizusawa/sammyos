
BUILD_DIR = build
BUILD_KERNEL_DIR = $(BUILD_DIR)/kernel
BUILD_BOOT_DIR = $(BUILD_DIR)/boot
BOOTSECT_BIN = $(BUILD_DIR)/boot/bootsect.bin
KERNEL_BIN = $(BUILD_DIR)/kernel/kernel.bin
KERNEL_ELF = $(BUILD_DIR)/kernel/kernel.elf
DISK_IMG = disk.img

all: run

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

run: bootdisk 
	qemu-system-i386 -fda $(DISK_IMG)

debug: bootdisk
	qemu-system-i386 -gdb tcp::8080  -fda $(DISK_IMG) -S &
	i386-elf-gdb -ex "target remote localhost:8080" -ex "symbol-file $(KERNEL_ELF)"

clean: 
	rm -rf build/

spotless: clean
	rm -f $(DISK_IMG)
