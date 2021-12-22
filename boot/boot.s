.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO 
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

# Declare multiboot header
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Allocate kernel stack
.section .bootstrap_stack, "aw", @nobits
.global stack_top

.align 16
stack_bottom:
    .skip 16384
stack_top:

# Preallocate space for paging
.section .bss, "aw", @nobits
.align 4096
boot_page_directory:
    .skip 4096
boot_page_table_1:
    .skip 4096

.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
    # Physical address of the first page table
    movl $(boot_page_table_1 - 0xC0000000), %edi
    movl $0, %esi
    movl $1023, %ecx

1:
    #cmp $_start_kernel, %esi
    #jl 2f
    cmpl $(_end_kernel - 0xC0000000), %esi
    jge 3f

    # Map physical adress as "present" and "writable"
    movl %esi, %edx
    orl $0x003, %edx
    movl %edx, (%edi)

2:
    addl $4096, %esi
    addl $4, %edi
    loop 1b

3:
    # Map VGA buffer from 0xB8000 to 0xC03FF000
    movl $(0x000B8000 | 0x003), boot_page_table_1 - 0xC0000000 + 1023 * 4

    movl $(boot_page_table_1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
    movl $(boot_page_table_1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4

    # Set cr3 to address of boot_page_directory
    movl $(boot_page_directory - 0xC0000000), %ecx
    movl %ecx, %cr3

    # Enable paging and write-protect bit
    movl %cr0, %ecx
    orl $0x80010000, %ecx
    movl %ecx, %cr0

    # Jump to higher half
    lea 4f, %ecx
    jmp *%ecx

.section .text

4:
    
    mov $stack_top, %esp
    andl $-16, %esp

    pushl %ebx
    pushl %eax
    
    call kernel_main

5:
    jmp 5b

# .size _start, . - _start
