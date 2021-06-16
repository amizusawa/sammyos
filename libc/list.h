#ifndef LIST_H
#define LIST_H

#include <stdint.h>

struct list_entry {
    void* value;
    struct list_entry* next;
    struct list_entry* prev;
};

struct list {
    struct list_entry head;
};

void list_init(struct list*);
void list_entry_init(struct list_entry*);
void list_push(struct list*, struct list_entry*);
void list_pop(struct list*, struct list_entry*);

#endif
