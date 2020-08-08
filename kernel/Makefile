
BUILD_DIR = ../build/kernel
CFLAGS = -g -O0 -ffreestanding -std=gnu99
C_SRCS = $(wildcard *.c)
C_OBJS = $(C_SRCS:.c=.o)
BUILD_C_OBJS = $(foreach file, $(C_OBJS), $(BUILD_DIR)/$(file))
MODULES = drivers
MODULE_C_OBJS = $(foreach dir, $(MODULES), $(wildcard ../build/$(dir)/*.o))
MODULE_C_HDRS = $(foreach dir, $(MODULES), $(wildcard ../$(dir)/*.h))

all: $(BUILD_C_OBJS)

$(BUILD_DIR)/%.o: %.c
	@ mkdir -p $(BUILD_DIR)
	i386-elf-gcc $(CFLAGS)  -c $< $(MODULE_OBJS) -o $@
