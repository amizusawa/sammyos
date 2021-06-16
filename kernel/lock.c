
#include <kernel/lock.h>
#include <stddef.h>

void lock_init(struct lock* lock) {
    
    lock->holder = NULL;
    lock->value = 1;
}

void lock_acquire(struct lock* lock) {
   
    // TODO: should fix busy wait later
    while (lock->value == 0);

    lock->value = 0;
    lock->holder = thread_current();
}

void lock_release(struct lock* lock) {
    
    lock->holder = NULL;
    lock->value = 1;
}


