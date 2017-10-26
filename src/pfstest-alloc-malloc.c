#include "pfstest-alloc.h"

#include <stdlib.h>
#include <string.h>

#include "pfstest-platform.h"
#include "pfstest-list.h"

#define ALIGNMENT PFSTEST_ALIGNMENT
#define ALIGN_MASK (~(ALIGNMENT - 1))

typedef struct _dynamic_env_t
{
    struct _dynamic_env_t *next;
    pfstest_list_t allocated;
} dynamic_env_t;

static dynamic_env_t default_dynamic_env = {
    NULL,
    PFSTEST_LIST_EMPTY()
};

static dynamic_env_t *dynamic_env = &default_dynamic_env;

/* Help uses of uninitialized memory fail early */
static void *evil_malloc(size_t size)
{
    void *r = malloc(size);
    memset(r, 0xaa, size);
    return r;
}

void *pfstest_alloc(size_t size)
{
    void *mem;
    pfstest_list_node_t *node;
    size_t header_size;

    if (size == 0)
        return NULL;

    header_size = sizeof(pfstest_list_node_t);
    if (header_size % ALIGNMENT) {
        header_size = (header_size + ALIGNMENT) & ALIGN_MASK;
    }
    pfstest_c_assert(header_size >= sizeof(pfstest_list_node_t));
    pfstest_c_assert(header_size <= sizeof(pfstest_list_node_t) + ALIGNMENT);

    node = evil_malloc(header_size + size);
    pfstest_c_assert(node != NULL);
    pfstest_list_node_init(node);

    pfstest_list_append(&dynamic_env->allocated, node);

    mem = (char *)node + header_size;
    pfstest_c_assert((pfstest_uintptr_t)mem % ALIGNMENT == 0);

    return mem;
}

void pfstest_alloc_free_frame(void)
{
    pfstest_list_node_t *node = pfstest_list_head(&dynamic_env->allocated);

    while (node != NULL) {
        pfstest_list_node_t *next = node->next;

        /* Help uses of freed memory fail early */
        memset(node, 0xaa, sizeof(*node) + 1);

        free(node);
        node = next;
    }

    pfstest_list_reset(&dynamic_env->allocated);
}

void pfstest_alloc_frame_push(void)
{
    dynamic_env_t *new_frame = evil_malloc(sizeof(*new_frame));
    pfstest_c_assert(new_frame != NULL);
    new_frame->next = dynamic_env;
    dynamic_env = new_frame;
    pfstest_list_reset(&dynamic_env->allocated);
}

void pfstest_alloc_frame_pop(void)
{
    dynamic_env_t *old_frame = dynamic_env;
    pfstest_alloc_free_frame();
    dynamic_env = old_frame->next;

    /* Help uses of freed memory fail early */
    memset(old_frame, 0xaa, sizeof(*old_frame));

    free(old_frame);
}
