include ${SAMMYOSROOT}/Makefile.vars

C_SRCS = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)
C_OBJS = $(C_SRCS:.c=.o)
BUILD_C_OBJS = $(foreach file, $(C_OBJS), $(BUILD_DIR)/libc/$(file))

all: $(BUILD_C_OBJS)

$(BUILD_DIR)/libc/%.o: %.c $(HEADERS)
	@ mkdir -p $(BUILD_DIR)/libc/src
	i386-elf-gcc $(CFLAGS) -ffreestanding -c $< -o $@
