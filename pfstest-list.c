#include "pfstest-list.h"

#include <stdlib.h>

void pfstest_list_append(pfstest_list_t *list, pfstest_list_node_t *node)
{
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

pfstest_list_node_t *pfstest_list_head(pfstest_list_t *list)
{
    return list->head;
}

void pfstest_list_reset(pfstest_list_t *list)
{
    list->head = NULL;
    list->tail = NULL;
}

void pfstest_list_node_init(pfstest_list_node_t *node)
{
    node->next = NULL;
}
