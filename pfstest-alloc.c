#include "pfstest-alloc.h"

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "pfstest-list.h"

static pfstest_list_t mem = PFSTEST_LIST_EMPTY();

struct alignment_struct 
{
    char c;
    union 
    {
        char c;
        short s;
        long l;
        long long ll;
        float f;
        double d;
        long double ld;
    } alignment_union;
};

#define ALIGNMENT offsetof(struct alignment_struct, alignment_union)
#define ALIGN_MASK (~(ALIGNMENT - 1))
#include <stdio.h>
void *pfstest_alloc(size_t size)
{
    size_t node_size = sizeof(pfstest_list_node_t);
    if (node_size % ALIGNMENT) {
        node_size = (node_size + ALIGNMENT) & ALIGN_MASK;
    }
    assert(node_size >= sizeof(pfstest_list_node_t));
    assert(node_size <= sizeof(pfstest_list_node_t) + ALIGNMENT);

    pfstest_list_node_t *node = malloc(node_size + size);
    assert(node != NULL);
    node->next = NULL;

    pfstest_list_append(&mem, node);
    intptr_t p = (intptr_t)node + node_size;
    assert(p % ALIGNMENT == 0);

    return (void *)p;
}

void pfstest_free_all(void)
{
    pfstest_list_node_t *node = pfstest_list_head(&mem);

    while (node != NULL) {
        pfstest_list_node_t *next = node->next;
        free(node);
        node = next;
    }

    pfstest_list_reset(&mem);
}
