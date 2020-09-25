#include "thread.h"
#include "frame_alloc.h"

uint32_t thread_id = 1;
static struct thread* initial_thread;
struct thread* thread_list[64];

struct thread* running_thread();

void init_thread() {
    initial_thread = running_thread();

}

uint32_t thread_create(const char* name, void* function) {
    struct thread* t;
   
    __asm__ __volatile__("cli");
    t = mmap_read(frame_alloc(), MMAP_GET_ADDR);
    return 0;
    
}

struct thread* running_thread() {
    uint32_t* esp;
    __asm__ __volatile__ ("mov %%esp, %0": "=g" (esp));
    uint32_t mask = (1 << 12) - 1;
    return (struct thread*) ((uintptr_t) esp & ~mask);
}
