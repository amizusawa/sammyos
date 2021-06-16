#ifndef LOCK_H
#define LOCK_H

#include <kernel/thread.h>

struct lock {
    int value;
    struct thread* holder;
};

void lock_init(struct lock*);
void lock_acquire(struct lock*);
void lock_release(struct lock*);

#endif
