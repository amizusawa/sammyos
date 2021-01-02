#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include <stddef.h>

#define MAX_THREADS 64
#define TIME_SLICE 1000

enum {
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_BLOCKED,
    THREAD_DYING,
};

struct thread {
    uint32_t stack_top;
    int state;
    uint32_t tid;
    uint32_t quantum;
};

typedef void thread_func(void *aux);

void init_thread();
uint32_t thread_create(thread_func* function);
void schedule();
struct thread* thread_current();
void thread_tick();
void thread_yield();

#endif
