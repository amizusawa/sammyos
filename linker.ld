
ENTRY(_start)

SECTIONS 
{
    . = 1M;

    _start_kernel = .;
    .multiboot.data : {
        *(.multiboot.data)
    }

    .multiboot.text : {
        *(.multiboot.text)
    }

    .text ALIGN (4K) : AT (ADDR (.text))
    {
        *(.text)
    }

    .rodata ALIGN (4K) : AT (ADDR (.rodata))
    {
        *(.rodata)
    }

    .data ALIGN (4K) : AT (ADDR (.data))
    {
        *(.data)
    }

    .bss ALIGN (4K) : AT (ADDR (.bss))
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }
    _end_kernel = .;
}
