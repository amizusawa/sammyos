
BUILD_DIR = ../build/cpu
C_SRCS = $(wildcard *.c)
HEADERS = $(wildcard *.h)
C_OBJS = $(C_SRCS:.c=.o)
S_SRCS = $(wildcard *.s)
S_OBJS = $(S_SRCS:.s=.o)
BUILD_C_OBJS = $(foreach file, $(C_OBJS), $(BUILD_DIR)/$(file))
BUILD_S_OBJS = $(foreach file, $(S_OBJS), $(BUILD_DIR)/$(file))
BUILD_OBJS = $(BUILD_S_OBJS) $(BUILD_C_OBJS)
CFLAGS = -g

all: $(BUILD_OBJS)

$(BUILD_DIR)/%.o: %.c $(HEADERS)
	@ mkdir -p $(BUILD_DIR)
	i386-elf-gcc $(CFLAGS) -ffreestanding -c $< -o $@

$(BUILD_DIR)/%.o: %.s
	@ mkdir -p $(BUILD_DIR) 
	nasm $< -f elf -o $@
