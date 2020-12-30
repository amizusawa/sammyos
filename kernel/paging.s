
.globl load_page_dir
.func load_page_dir
load_page_dir:
    pushl %ebp
    movl %esp, %ebp
    movl 8(%esp), %eax
    movl %eax, %cr3
    movl %ebp, %esp
    popl %ebp
    ret
.endfunc

.globl enable_paging
.func enable_paging
enable_paging:
    pushl %ebp
    movl %esp, %ebp
    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0
    movl %ebp, %esp
    popl %ebp
    ret
.endfunc
