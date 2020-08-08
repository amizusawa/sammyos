
BUILD_DIR = ../build/boot

all: $(BUILD_DIR)/boot.o

$(BUILD_DIR)/boot.o: boot.s
	@ mkdir -p $(BUILD_DIR)
	i386-elf-as boot.s -o $@
