#include "thread.h"
#include "switch.h"
#include <cpu/interrupt.h>
#include <mm/pmm.h>
#include <drivers/screen.h>
#include <function.h>

struct thread* current_thread;
struct thread* init_task;
struct thread* idle_thread;

uint32_t number_tasks = 0;
struct thread* thread_list[MAX_THREADS];
extern uint32_t stack_top;

uint32_t allocate_tid();
static void* alloc_stack_frame(struct thread* t, uint32_t size);
void thread_schedule_tail(struct thread* prev);
struct thread* running_thread();
static void kernel_thread(thread_func* func, void* aux);
static void idle();
void schedule();
uint32_t idle_thread_id = 0;

struct kernel_thread_frame {
    void* eip;
    thread_func* function;
    void* aux;
};

void init_thread() {
    init_task->stack_top = (uint8_t*) stack_top;
    init_task->state = THREAD_RUNNING;
    init_task->tid = allocate_tid();

    thread_list[init_task->tid] = init_task;
}

void thread_start() {
    thread_create(idle);

    intr_enable();
    schedule();
}

uint32_t allocate_tid() {
    if (number_tasks >= MAX_THREADS) return -1; 
    else return number_tasks++;
}

uint32_t thread_create(thread_func* function) {
    
    void* pf = pmm_get_page(PMM_ZERO);
    if (!pf) {
        return -1;
    }

    struct thread* t = pf;
    struct kernel_thread_frame* kf;
    struct switch_entry_frame* ef;
    struct switch_threads_frame* sf;

    t->stack_top = (uint8_t*) pf + PAGE_SIZE;
    t->tid = allocate_tid();
    t->quantum = TIME_SLICE;
    t->state = THREAD_READY;

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

    if (function == idle) {
        idle_thread = t;
        idle_thread_id = t->tid;
    }
    return t->tid;
}

void thread_exit() {
    // TODO: mark current thread as dying
    struct thread* current = thread_current();
    current->state = THREAD_DYING;
    thread_list[current->tid] = NULL;
    schedule();
}

void thread_tick() {
    struct thread* current = thread_current();

    if (current == init_task) {
        return;
    }

    current->quantum--;
    
    if (current->quantum <= 0) {
        intr_yield_on_return();
    }
}

void thread_block() {
    thread_current()->state = THREAD_BLOCKED;

    //schedule();
}

struct thread* thread_current() {
    struct thread* t = running_thread();
    // TODO: check for stack overflow 
    return t;
}

void thread_yield() {
    struct thread* cur = thread_current();
    cur->state = THREAD_READY;

    schedule();
}

static void idle() {
    kprint("idle");
    idle_thread = thread_current();

    for (;;) {
        //intr_disable();
        //thread_block();

        asm volatile ("sti; hlt" : : : "memory");
    }
}

static void kernel_thread(thread_func* func, void* aux) {
    intr_enable();
    func(aux);
    thread_exit();
}

void schedule() {
    intr_disable();
    struct thread* next = NULL;
    struct thread* current = thread_current();
    struct thread* prev = NULL;

    /*
    for (uint32_t i = 1; i < MAX_THREADS; i++) {
        struct thread* t = thread_list[i];
        if (t && t->state == THREAD_READY && i != last_thread_id) {
            next = t;
            last_thread_id = i;
            current->state = THREAD_READY;

            break;
        }
    }
    */

    uint32_t i = (current->tid + 1) % MAX_THREADS;
    while (i != current->tid) {
        if (i == idle_thread_id || i == 0) {
            i = (i + 1) % MAX_THREADS;
            continue;
        }

        struct thread* t = thread_list[i];
        if (t && t->state == THREAD_READY) {
            next = t;
            // TODO: only change current's status to ready if it's still running
            current->state = THREAD_READY;
        }
        i = (i + 1) % MAX_THREADS;
    }


    if (!next) {
        next = idle_thread;
    }
    prev = switch_threads(current, next);
    thread_schedule_tail(prev);
}

void thread_schedule_tail(struct thread* prev) {
    UNUSED(prev);

    struct thread* current = thread_current();
    current->state = THREAD_RUNNING;
    current->quantum = TIME_SLICE;
    // TODO: free and destroy prev if it's dying, mark current thread as running
}

static void* alloc_stack_frame(struct thread* t, uint32_t size) {
    t->stack_top -= size;
    return (void*) t->stack_top;
}

struct thread* running_thread() {
    uint32_t* esp;

    asm("mov %%esp, %0" : "=g" (esp));
    return pg_round_down(esp);
}


uint32_t thread_stack_ofs = offsetof(struct thread, stack_top);
