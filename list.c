#include "list.h"

#include <stdlib.h>

void list_append(struct list *list, struct list_node *node)
{
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

struct list_node *list_head(struct list *list)
{
    return list->head;
}
