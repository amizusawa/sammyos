
#include <kernel/lock/lock.h>
#include <stddef.h>

void lock_init(struct lock* lock) {
    
    lock->holder = NULL;
    lock->value = 0;
}

int test_and_set(int* old_ptr, int new) {
    int old = *old_ptr;
    *old_ptr = new;
    return old;
}

void lock_acquire(struct lock* lock) {
   
    // TODO: should fix busy wait later
    while (test_and_set(&lock->value, 1) == 1);
    lock->holder = thread_current();
}

void lock_release(struct lock* lock) {
    
    lock->holder = NULL;
    lock->value = 0;
}


