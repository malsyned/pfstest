#ifndef PFSTEST_LIST_H
#define PFSTEST_LIST_H

typedef struct _pfstest_list_node pfstest_list_node_t;

struct _pfstest_list_node
{
    pfstest_list_node_t *next;
};

typedef struct
{
    pfstest_list_node_t *head;
    pfstest_list_node_t *tail;
} pfstest_list_t;

#define PFSTEST_LIST_EMPTY() {NULL, NULL}

void pfstest_list_append(pfstest_list_t *list, pfstest_list_node_t *node);
pfstest_list_node_t *pfstest_list_head(pfstest_list_t *list);

#endif /* !PFSTEST_LIST_H */
