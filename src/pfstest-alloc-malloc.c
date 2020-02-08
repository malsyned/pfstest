#include "pfstest-alloc.h"

#include <stdlib.h>
#include <string.h>

#include "pfstest-platform.h"
#include "pfstest-list.h"

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

static void corrupt(void *mem, size_t size)
{
    /* Help uses of uninitialized or already-freed memory fail early */
    memset(mem, 0xaa, size);
}

static void corrupt_and_free(void *mem, size_t size)
{
    corrupt(mem, size);
    free(mem);
}

static void *evil_malloc(size_t size)
{
    void *mem = malloc(size);
    pfstest_c_assert(mem != NULL);
    corrupt(mem, size);
    return mem;
}

static pfstest_bool aligned(pfstest_uintptr_t n)
{
    return (n % PFSTEST_ALIGNMENT == 0);
}

static size_t align_size(size_t size)
{
    size_t aligned_size;

    if (!aligned(size))
        aligned_size = size + (PFSTEST_ALIGNMENT - size % PFSTEST_ALIGNMENT);
    else
        aligned_size = size;

    pfstest_c_assert(aligned(aligned_size));
    pfstest_c_assert(aligned_size >= size);
    pfstest_c_assert(aligned_size < size + PFSTEST_ALIGNMENT);

    return aligned_size;
}

void *pfstest_alloc(size_t size)
{
    pfstest_list_node_t *node;
    size_t header_size;
    void *mem;

    if (size == 0)
        return NULL;

    header_size = align_size(sizeof(pfstest_list_node_t));

    node = evil_malloc(header_size + size);
    mem = (char *)node + header_size;

    pfstest_list_node_init(node);
    pfstest_list_append(&dynamic_env->allocated, node);

    return mem;
}

void pfstest_alloc_free_frame(void)
{
    pfstest_list_node_t *node, *next;

    /* Can't use pfstest_list_iter, it would use-after-free */
    for (node = pfstest_list_head(&dynamic_env->allocated);
         node != NULL; node = next)
    {
        next = node->next;
        corrupt_and_free(node, sizeof(*node) + 1);
    }

    pfstest_list_reset(&dynamic_env->allocated);
}

void pfstest_alloc_frame_push(void)
{
    dynamic_env_t *new_frame = evil_malloc(sizeof(*new_frame));
    new_frame->next = dynamic_env;
    dynamic_env = new_frame;
    pfstest_list_reset(&dynamic_env->allocated);
}

void pfstest_alloc_frame_pop(void)
{
    dynamic_env_t *old_frame = dynamic_env;
    pfstest_alloc_free_frame();
    dynamic_env = old_frame->next;

    corrupt_and_free(old_frame, sizeof(*old_frame));
}
