include ${SAMMYOSROOT}/Makefile.vars

COMPONENTS = thread mm lock misc drivers
$(foreach component, $(COMPONENTS), $(shell mkdir -p $(BUILD_DIR)/kernel/src/$(component)))
C_SRCS += $(foreach component, $(COMPONENTS), $(wildcard src/$(component)/*.c))
S_SRCS += $(foreach component, $(COMPONENTS), $(wildcard src/$(component)/*.s))
C_SRCS += $(wildcard src/*.c)
S_SRCS += $(wildcard src/*.s)

C_OBJS = $(C_SRCS:.c=.o)
S_OBJS = $(S_SRCS:.s=.o)

BUILD_C_OBJS += $(foreach file, $(C_OBJS), $(BUILD_DIR)/kernel/$(file))
BUILD_S_OBJS += $(foreach file, $(S_OBJS), $(BUILD_DIR)/kernel/$(file))
BUILD_OBJS = $(BUILD_S_OBJS) $(BUILD_C_OBJS)

all: $(BUILD_OBJS) arch

$(BUILD_DIR)/kernel/%.o: %.c
	i386-elf-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/kernel/%.o: %.s
	i386-elf-as $< -o $@

arch:
	make -C src/arch/${SAMMYOSARCH}
