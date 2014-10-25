#include "pfstest-alloc.h"

#include <stddef.h>

#include "pfstest-platform.h"

#define HEAP_SIZE ((int)2048)

#pragma udata heap
unsigned char heap[HEAP_SIZE];
#pragma udata

static unsigned char *next_free_byte = heap;

void *pfstest_alloc(size_t size)
{
    void *r = NULL;
    ptrdiff_t used = next_free_byte - heap;
    int free = HEAP_SIZE - (int)used;

    assert(free >= size);

    r = next_free_byte;
    next_free_byte += size;

    return r;
}

void pfstest_free_all(void)
{
    next_free_byte = heap;
}
