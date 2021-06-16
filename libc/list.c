
#include <list.h>
#include <stddef.h>
#include <mem.h>

void list_init(struct list* list) {
    list->head.value = NULL;
    list->head.prev = NULL;
    list->head.next = NULL;
}
