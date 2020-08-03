
BUILD_DIR = ../build/drivers
C_SRCS = $(wildcard *.c)
C_OBJS = $(C_SRCS:.c=.o)
BUILD_C_OBJS = $(foreach file, $(C_OBJS), $(BUILD_DIR)/$(file))
CFLAGS = -g

all: $(BUILD_C_OBJS)

$(BUILD_DIR)/%.o: %.c
	i386-elf-gcc $(CFLAGS) -ffreestanding -c $< -o $@
