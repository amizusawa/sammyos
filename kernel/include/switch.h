#ifndef SWITCH_H
#define SWITCH_H

#include <stdint.h>
#include "thread.h"

#ifndef __ASSEMBLER__
struct switch_threads_frame {
    uint32_t edi;    
    uint32_t esi;    
    uint32_t ebp;    
    uint32_t ebx; 
    void (*eip) (void);
    struct thread* cur;
    struct thread* next;
};

struct thread* switch_threads(struct thread* cur, struct thread* next);

struct switch_entry_frame {
    void (*eip) (void);   
};

void switch_entry();

void switch_thunk();

#endif

#define SWITCH_CUR 20;
#define SWITCH_NEXT 24;

#endif
