
BUILD_DIR = ../build/kernel
CFLAGS = -g -O0
C_SRCS = $(wildcard *.c)
C_OBJS = $(C_SRCS:.c=.o)
BUILD_C_OBJS = $(foreach file, $(C_OBJS), $(BUILD_DIR)/$(file))
MODULES = drivers
MODULE_C_OBJS = $(foreach dir, $(MODULES), $(wildcard ../build/$(dir)/*.o))

all: $(BUILD_DIR)/kernel.bin $(BUILD_DIR)/kernel.elf

$(BUILD_DIR)/%.o: %.c
	@ mkdir -p $(BUILD_DIR)
	i386-elf-gcc $(CFLAGS) -ffreestanding -c $< -o $@

$(BUILD_DIR)/kernel_entry.o: kernel_entry.asm
	@ mkdir -p $(BUILD_DIR)
	nasm $< -f elf -o $@

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/kernel_entry.o $(BUILD_C_OBJS)
	i386-elf-ld -o $@ -Ttext 0x1000 $^ $(MODULE_OBJS)

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel_entry.o $(BUILD_C_OBJS)
	i386-elf-ld -o $@ -Ttext 0x1000 $^ $(MODULE_OBJS) --oformat binary
