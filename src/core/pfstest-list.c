/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest-list.h"

#include <stdlib.h>

void pfstest_list_append(pfstest_list_t *list, pfstest_list_node_t *node)
{
    pfstest_list_node_t *tail;

    if (list->head == NULL) {
        list->head = node;
    } else {
        for (tail = list->head; tail->next != NULL; tail = tail->next) {
            /* Bail if the node is already in the list */
            if (tail == node)
                return;
        }
        tail->next = node;
    }
    pfstest_list_node_init(node);
}

pfstest_list_node_t *pfstest_list_head(pfstest_list_t *list)
{
    return list->head;
}

void pfstest_list_reset(pfstest_list_t *list)
{
    list->head = NULL;
}

void pfstest_list_node_init(pfstest_list_node_t *node)
{
    node->next = NULL;
}
