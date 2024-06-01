
include Makefile.vars

$(info Target architecture: ${SAMMYOSARCH})

ARCH_DIR = arch/${SAMMYOSARCH}
MODULES = kernel libc
OBJS = $(shell find $(BUILD_DIR) -type f -name "*.o")
ISO_DIR = isodir
OS = sammyos
ISO = $(OS).iso
OS_BIN = $(OS).bin

ifneq (, $(shell command -v grub-mkrescue))
$(info Using grub-mkrescue)
GRUB_MKRESCUE = grub-mkrescue
else ifneq (, $(shell command -v grub2-mkrescue))
$(info Using grub2-mkrescue)
GRUB_MKRESCUE = grub2-mkrescue
else
$(error "Grub not installed.")
endif

all: iso 

.PHONY: run iso bootsect clean debug $(MODULES)

define make-module =
$(1):
	make -C $(1)
endef

$(foreach module, $(MODULES), $(eval $(call make-module,$(module))))

iso: $(MODULES)
	i386-elf-gcc -T linker.ld -o $(BUILD_DIR)/$(OS_BIN) $(LINKER_FLAGS) $(OBJS) -lgcc
	mkdir -p $(BUILD_DIR)/$(ISO_DIR)/boot/grub
	cp $(BUILD_DIR)/$(OS_BIN) $(BUILD_DIR)/$(ISO_DIR)/boot/$(OS_BIN)
	cp grub.cfg $(BUILD_DIR)/$(ISO_DIR)/boot/grub/grub.cfg
	${GRUB_MKRESCUE} -o $(BUILD_DIR)/$(ISO) $(BUILD_DIR)/$(ISO_DIR)

run: 
	qemu-system-i386 -s -cdrom $(BUILD_DIR)/$(ISO)

debug:
	qemu-system-i386 -s -S -cdrom $(BUILD_DIR)/$(ISO) &
	sleep 0.5s
	gdb

clean: 
	rm -rf $(BUILD_DIR)

