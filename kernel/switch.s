#include "switch.h"

# void switch_threads(struct thread* current, struct thread* next)
.globl switch_threads
.func switch_threads
switch_threads:
    pushl %ebx
    pushl %esi
    pushl %edi
    pushl %ebp

.globl thread_stack_ofs
    # current->stack = esp
    mov thread_stack_ofs, %edx
    movl 20(%esp), %eax
    movl %esp, (%eax,%edx,1)

    #esp = next->stack
    movl 24(%esp), %ecx
    movl (%ecx,%edx,1), %esp

    popl %edi
    popl %esi
    popl %ebp
    popl %ebx

    ret
.endfunc

.globl switch_entry
.func switch_entry
switch_entry:
    addl $8, %esp
    pushl %eax
.globl thread_schedule_tail
    call thread_schedule_tail
    addl $4, %esp

    ret
.endfunc


