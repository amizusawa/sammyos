
[GLOBAL load_page_dir]
load_page_dir:
    push ebp
    mov ebp, esp
    mov eax, [8+esp]
    mov cr3, eax
    mov esp, ebp
    pop ebp
    ret

[GLOBAL enable_paging]
enable_paging:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    mov esp, ebp
    pop ebp
    ret
