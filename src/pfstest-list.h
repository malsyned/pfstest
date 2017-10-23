#ifndef PFSTEST_LIST_H
#define PFSTEST_LIST_H

#include <stddef.h>

typedef struct _pfstest_list_node pfstest_list_node_t;

struct _pfstest_list_node
{
    pfstest_list_node_t *next;
};

typedef struct
{
    pfstest_list_node_t *head;
} pfstest_list_t;

#define PFSTEST_LIST_EMPTY() {NULL}

void pfstest_list_append(pfstest_list_t *list, pfstest_list_node_t *node);
pfstest_list_node_t *pfstest_list_head(pfstest_list_t *list);
void pfstest_list_reset(pfstest_list_t *list);
void pfstest_list_node_init(pfstest_list_node_t *node);

#define pfstest_list_iter(node, list)                           \
    for (node = (void *)pfstest_list_head(list);                \
         node != NULL;                                          \
         node = (void *)((pfstest_list_node_t *)node)->next)

#endif /* !PFSTEST_LIST_H */
