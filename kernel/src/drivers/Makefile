include ${SAMMYOSROOT}/Makefile.vars

C_SRCS = $(wildcard *.c)
HEADERS = $(wildcard *.h)
C_OBJS = $(C_SRCS:.c=.o)
BUILD_C_OBJS = $(foreach file, $(C_OBJS), $(BUILD_DIR)/drivers/$(file))

all: $(BUILD_C_OBJS)

$(BUILD_DIR)/drivers/%.o: %.c $(HEADERS)
	@ mkdir -p $(BUILD_DIR)/drivers
	i386-elf-gcc $(CFLAGS) -ffreestanding -c $< -o $@
