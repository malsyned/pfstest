#ifndef LIST_H
#define LIST_H

struct list_node
{
    struct list_node *next;
};

struct list
{
    struct list_node *head;
    struct list_node *tail;
};

#define LIST_EMPTY() {NULL, NULL}

void list_append(struct list *list, struct list_node *node);
struct list_node *list_head(struct list *list);

#endif /* !LIST_H */
