#include "thread.h"
#include "switch.h"
#include <cpu/interrupt.h>
#include <mm/frame_alloc.h>
#include <drivers/screen.h>
#include <function.h>

struct thread* current_thread;
struct thread* init_task;

uint32_t number_tasks = 0;
struct thread* thread_list[MAX_THREADS];
extern uint32_t stack_top;

uint32_t allocate_tid();
static void* alloc_stack_frame(struct thread* t, uint32_t size);
struct thread* running_thread();
static void kernel_thread(thread_func* func, void* aux);

struct kernel_thread_frame {
    void* eip;
    thread_func* function;
    void* aux;
};

void init_thread() {
    init_task->stack_top = stack_top;
    init_task->state = THREAD_RUNNING;
    init_task->tid = allocate_tid();

    thread_list[init_task->tid] = init_task;
}

uint32_t allocate_tid() {
    if (number_tasks >= MAX_THREADS) return -1; 
    else return number_tasks++;
}

uint32_t thread_create(thread_func* function) {
    
    struct page_frame* pf = frame_alloc();
    if (!pf) {
        return -1;
    }

    struct thread* t = pf->page_addr;
    struct kernel_thread_frame* kf;
    struct switch_entry_frame* ef;
    struct switch_threads_frame* sf;

    t->stack_top = (uint32_t) pf->page_addr + PAGE_SIZE;
    t->tid = allocate_tid();
    t->quantum = TIME_SLICE;

    kf = alloc_stack_frame(t, sizeof(*kf));
    kf->eip = NULL;
    kf->function = function;
    kf->aux = NULL;

    ef = alloc_stack_frame(t, sizeof(*ef));
    ef->eip = (void (*) (void)) kernel_thread;

    sf = alloc_stack_frame(t, sizeof(*sf));
    sf->eip = switch_entry;
    sf->ebp = 0;

    thread_list[t->tid] = t;
    return t->tid;
}

void thread_exit() {
    // TODO: mark current thread as dying
    schedule();
}

void thread_tick() {
    struct thread* current = thread_current();
    current->quantum--;
    
    if (current->quantum <= 0) {
        intr_yield_on_return();
    }
}

struct thread* thread_current() {
    struct thread* t = running_thread();
    // TODO: check for stack overflow 
    return t;
}

static void kernel_thread(thread_func* func, void* aux) {
    intr_enable();
    func(aux);
    thread_exit();
}

int temp_var = 1;
void schedule() {
    // TODO: implement actual scheduling
    if (temp_var == 1) {
        struct thread* next = thread_list[temp_var--];
        struct thread* current = running_thread();  

        switch_threads(current, next);
    }
    else {
        kprint("idle");
        for (;;);
    }
}

void thread_schedule_tail(struct thread* prev) {
    UNUSED(prev);
    // TODO: free and destroy prev if it's dying, mark current thread as running
}

static void* alloc_stack_frame(struct thread* t, uint32_t size) {
    t->stack_top -= size;
    return (void*) t->stack_top;
}

struct thread* running_thread() {
    uint32_t* esp;

    asm("mov %%esp, %0" : "=g" (esp));
    return page_round_down(esp);
}


uint32_t thread_stack_ofs = offsetof(struct thread, stack_top);
