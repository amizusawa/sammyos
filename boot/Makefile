
BUILD_DIR = ../build/boot
BOOTSECT = bootsect.asm
BOOTSECT_BIN = $(BOOTSECT:.asm=.bin)
MODULES = 32_bit_gdt switch_to_pm print print_hex disk
MODULE_SRCS = $(MODULES:=.asm)

all: bootsect


bootsect: $(BOOTSECT) $(MODULE_SRCS) 
	@ mkdir -p $(BUILD_DIR) 
	nasm -f bin $(BOOTSECT) -o $(BUILD_DIR)/$(BOOTSECT_BIN)
