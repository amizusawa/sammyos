include ${SAMMYOSROOT}/Makefile.vars

C_SRCS = $(wildcard cpu/*.c)
HEADERS = $(wildcard cpu/*.h)
C_OBJS = $(C_SRCS:.c=.o)
S_SRCS = $(wildcard cpu/*.s)
S_OBJS = $(S_SRCS:.s=.o)
BUILD_C_OBJS = $(foreach file, $(C_OBJS), $(BUILD_DIR)/arch/i386/$(file))
BUILD_S_OBJS = $(foreach file, $(S_OBJS), $(BUILD_DIR)/arch/i386/$(file))
BUILD_OBJS = $(BUILD_S_OBJS) $(BUILD_C_OBJS)

all: $(BUILD_OBJS) $(BUILD_DIR)/arch/i386/boot.o

$(BUILD_DIR)/arch/i386/%.o: %.c $(HEADERS)
	@ mkdir -p $(BUILD_DIR)/arch/i386/cpu
	i386-elf-gcc $(CFLAGS) -ffreestanding -c $< -o $@

$(BUILD_DIR)/arch/i386/%.o: %.s
	@ mkdir -p $(BUILD_DIR)/arch/i386/cpu
	nasm $< -f elf -o $@

$(BUILD_DIR)/arch/i386/boot.o: boot/boot.s
	@ mkdir -p $(BUILD_DIR)/arch/i386/boot
	i386-elf-as boot/boot.s -o $@
