include ${SAMMYOSROOT}/Makefile.vars

COMPONENTS = thread mm lock misc
$(foreach component, $(COMPONENTS), $(shell mkdir -p $(BUILD_DIR)/kernel/$(component)))
C_SRCS += $(foreach component, $(COMPONENTS), $(wildcard $(component)/*.c))
S_SRCS += $(foreach component, $(COMPONENTS), $(wildcard $(component)/*.s))
C_SRCS += $(wildcard *.c)
S_SRCS += $(wildcard *.s)

C_OBJS = $(C_SRCS:.c=.o)
S_OBJS = $(S_SRCS:.s=.o)

BUILD_C_OBJS += $(foreach file, $(C_OBJS), $(BUILD_DIR)/kernel/$(file))
BUILD_S_OBJS += $(foreach file, $(S_OBJS), $(BUILD_DIR)/kernel/$(file))
BUILD_OBJS = $(BUILD_S_OBJS) $(BUILD_C_OBJS)

all: $(BUILD_OBJS)

$(BUILD_DIR)/kernel/%.o: %.c
	i386-elf-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel/%.o: %.s
	i386-elf-as $< -o $@

