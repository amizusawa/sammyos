
[org 0x7c00]
KERNEL_OFFSET equ 0x1000

    cli
    mov [BOOT_DRIVE], dl
    mov bp, 0x9000
    mov sp, bp

    mov ah, 06h
    mov al, 0
    mov bh, 7
    mov ch, 0
    mov cl, 0
    mov dh, 24
    mov dl, 79
    int 10h

    ;mov ah, 01h
    ;mov ch, 3fh
    ;int 10h

    call load_kernel
    call switch_to_pm

    jmp $

%include "print.asm"
%include "print_hex.asm"
%include "disk.asm"
%include "32_bit_gdt.asm"
%include "32_bit_print.asm"
%include "switch_to_pm.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, 16
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    mov ebx, PM_MSG
    call print_string_pm
    call KERNEL_OFFSET
    jmp $

BOOT_DRIVE db 0
PM_MSG db "In 32-bit protected mode", 0
times 510-($-$$) db 0
dw 0xaa55
