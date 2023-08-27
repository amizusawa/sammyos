
include Makefile.vars

$(info Target architecture: ${SAMMYOSARCH})

ARCH_DIR = arch/${SAMMYOSARCH}
MODULES = ${ARCH_DIR}/boot ${ARCH_DIR}/cpu kernel drivers libc
#OBJS = $(foreach dir, $(MODULES), $(wildcard $(BUILD_DIR)/$(dir)/*.o))
OBJS = $(shell find $(BUILD_DIR) -type f -name "*.o")
ISO_DIR = isodir
OS = sammyos
ISO = $(OS).iso
OS_BIN = $(OS).bin

all: iso 

.PHONY: run iso bootsect clean debug $(MODULES)

define make-module =
$(1):
	make -C $(1)
endef

$(foreach module, $(MODULES), $(eval $(call make-module,$(module))))

iso: $(MODULES)
	i386-elf-gcc -T linker.ld -o $(OS_BIN) $(LINKER_FLAGS) $(OBJS) -lgcc
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(OS_BIN) $(ISO_DIR)/boot/$(OS_BIN)
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

run: 
	qemu-system-i386 -s -cdrom $(ISO)

debug:
	qemu-system-i386 -s -S -cdrom $(ISO) &
	sleep 0.5s
	i386-elf-gdb

clean: 
	rm -rf $(BUILD_DIR)
	rm -rf $(ISO_DIR)
	rm -f $(OS).*

