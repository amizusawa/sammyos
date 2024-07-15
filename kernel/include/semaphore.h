
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <list.h>

struct semaphore {
    uint32_t value;
    struct list waiters;
};

void semaphore_init(struct semaphore* sem);
void semaphore_post(struct semaphore* sem);
void semaphore_wait(struct semaphore* sem);

#endif
