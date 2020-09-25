#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>

enum thread_status {
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_BLOCKED
};

struct cpu_context {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t ebx;
    void (*eip) (void);
};

struct thread {
    uint32_t tid;
    char name[16];
    enum thread_status status;
    struct cpu_context context;
};

void init_thread();
uint32_t thread_create(const char* name, void* function);

#endif
