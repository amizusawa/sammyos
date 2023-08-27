include ${SAMMYOSROOT}/Makefile.vars

all: $(BUILD_DIR)/boot/boot.o

$(BUILD_DIR)/boot/boot.o: boot.s
	@ mkdir -p $(BUILD_DIR)/boot
	i386-elf-as boot.s -o $@
