#include "list.h"

#include <stdlib.h>

void list_append(list_t *list, list_node_t *node)
{
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

list_node_t *list_head(list_t *list)
{
    return list->head;
}
