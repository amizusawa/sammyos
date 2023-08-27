include ${SAMMYOSROOT}/Makefile.vars

C_SRCS = $(wildcard *.c)
HEADERS = $(wildcard *.h)
C_OBJS = $(C_SRCS:.c=.o)
S_SRCS = $(wildcard *.s)
S_OBJS = $(S_SRCS:.s=.o)
BUILD_C_OBJS = $(foreach file, $(C_OBJS), $(BUILD_DIR)/cpu/$(file))
BUILD_S_OBJS = $(foreach file, $(S_OBJS), $(BUILD_DIR)/cpu/$(file))
BUILD_OBJS = $(BUILD_S_OBJS) $(BUILD_C_OBJS)

all: $(BUILD_OBJS)

$(BUILD_DIR)/cpu/%.o: %.c $(HEADERS)
	@ mkdir -p $(BUILD_DIR)/cpu
	i386-elf-gcc $(CFLAGS) -ffreestanding -c $< -o $@

$(BUILD_DIR)/cpu/%.o: %.s
	@ mkdir -p $(BUILD_DIR)/cpu
	nasm $< -f elf -o $@
