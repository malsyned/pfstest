#include "pfstest-alloc-pic.h"

#include <stddef.h>
#include <string.h>

#include "pfstest-platform.h"

typedef struct _dynamic_env_t
{
    struct _dynamic_env_t *next;
    unsigned char *next_free_byte;
} dynamic_env_t;

unsigned char *heap_end = NULL;
dynamic_env_t *dynamic_env = NULL;

static void assert_object_fits_in_heap(void *o, size_t size)
{
    unsigned char *last_fitting_address = heap_end - (size - 1);

    pfstest_assert((unsigned char *)o <= last_fitting_address);
}

static unsigned char *get_frame_start(dynamic_env_t *env)
{
    return (unsigned char *)env + sizeof(*env);
}

static void push_new_env(dynamic_env_t *new_env)
{
    new_env->next = dynamic_env;
    new_env->next_free_byte = get_frame_start(new_env);
    dynamic_env = new_env;
}

static void *get_next_object(size_t size)
{
    void *r = (void *)dynamic_env->next_free_byte;
    assert_object_fits_in_heap(r, size);
    return r;
}

void pfstest_alloc_pic_init(unsigned char *heap, size_t size)
{
    heap_end = heap + (size - 1);
    assert_object_fits_in_heap(heap, sizeof(dynamic_env_t));
    push_new_env((void *)heap);
}

void *pfstest_alloc(size_t size)
{
    void *r = get_next_object(size);
    dynamic_env->next_free_byte += size;
    return r;
}

void pfstest_alloc_free_frame(void)
{
    unsigned char *frame_start = get_frame_start(dynamic_env);

    /* Help uses of freed memory fail early */
    memset(frame_start, 0xaa, dynamic_env->next_free_byte - frame_start);
    dynamic_env->next_free_byte = frame_start;
}

void pfstest_alloc_frame_push(void)
{
    push_new_env(get_next_object(sizeof(dynamic_env_t)));
}

void pfstest_alloc_frame_pop(void)
{
    dynamic_env_t *old_frame = dynamic_env;
    pfstest_alloc_free_frame();

    dynamic_env = old_frame->next;

    /* Help uses of freed memory fail early */
    memset(old_frame, 0xaa, sizeof(*old_frame));
}
