include ../Makefile.vars

BUILD_DIR = ../build/kernel
C_SRCS = $(wildcard *.c)
C_OBJS = $(C_SRCS:.c=.o)
S_SRCS = $(wildcard *.s)
S_OBJS = $(S_SRCS:.s=.o)
BUILD_C_OBJS = $(foreach file, $(C_OBJS), $(BUILD_DIR)/$(file))
BUILD_S_OBJS = $(foreach file, $(S_OBJS), $(BUILD_DIR)/$(file))
BUILD_OBJS = $(BUILD_S_OBJS) $(BUILD_C_OBJS)

all: $(BUILD_OBJS)

$(BUILD_DIR)/%.o: %.c
	@ mkdir -p $(BUILD_DIR)
	i386-elf-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s
	@ mkdir -p $(BUILD_DIR) 
	i386-elf-as $< -o $@

