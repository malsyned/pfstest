#ifndef LIST_H
#define LIST_H

typedef struct _list_node list_node_t;

struct _list_node
{
    list_node_t *next;
};

typedef struct
{
    list_node_t *head;
    list_node_t *tail;
} list_t;

#define LIST_EMPTY() {NULL, NULL}

void list_append(list_t *list, list_node_t *node);
list_node_t *list_head(list_t *list);

#endif /* !LIST_H */
