#include "pfstest-alloc.h"

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "pfstest-list.h"

static pfstest_list_t allocated = PFSTEST_LIST_EMPTY();

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

void *pfstest_alloc(size_t size)
{
    void *mem;
    size_t header_size = sizeof(pfstest_list_node_t);
    if (header_size % ALIGNMENT) {
        header_size = (header_size + ALIGNMENT) & ALIGN_MASK;
    }
    assert(header_size >= sizeof(pfstest_list_node_t));
    assert(header_size <= sizeof(pfstest_list_node_t) + ALIGNMENT);

    pfstest_list_node_t *node = malloc(header_size + size);
    assert(node != NULL);
    pfstest_list_node_init(node);

    pfstest_list_append(&allocated, node);

    mem = (char *)node + header_size;
    assert((intptr_t)mem % ALIGNMENT == 0);

    return mem;
}

void pfstest_free_all(void)
{
    pfstest_list_node_t *node = pfstest_list_head(&allocated);

    while (node != NULL) {
        pfstest_list_node_t *next = node->next;
        free(node);
        node = next;
    }

    pfstest_list_reset(&allocated);
}
