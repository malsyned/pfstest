#include "pfstest-avr-mem.h"

#include <assert.h>

static const unsigned char fill_chars[] = "EMTY";
#define get_fill_char(i) (fill_chars[(i) & 0x3])

#if defined(__AVR__)

#include <avr/io.h>

extern unsigned char __heap_start;
extern char *__brkval;
extern char *__malloc_heap_start;

static unsigned char *get_stack_pointer(void)
{
    return ((unsigned char *)AVR_STACK_POINTER_REG);
}

void pfstest_avr_fill_heap_with_sentinel(void)
{
    _pfstest_avr_fill_mem_with_sentinel(&__heap_start, get_stack_pointer);
}

pfstest_avr_mem_usage_t pfstest_avr_max_heap_and_stack_usage(void)
{
    size_t heap_size =
        (size_t)((unsigned char *)(RAMEND + 1) - &__heap_start);
    return _pfstest_avr_mem_usage(&__heap_start, heap_size,
                                  get_stack_pointer);
}

size_t pfstest_avr_malloc_used(void)
{
    if (__brkval == 0)          /* Uninitialized case */
        return 0;
    else
        return (size_t)(__brkval - __malloc_heap_start);
}

#endif

void _pfstest_avr_fill_mem_with_sentinel(unsigned char *heap,
                                         unsigned char *(*get_sp)(void))
{
    size_t i;
    unsigned char *sp = get_sp();
    unsigned char *p = heap;

    i = 0;
    while (p <= sp) {
        *p++ = get_fill_char(i++);
    }
}

pfstest_avr_mem_usage_t _pfstest_avr_mem_usage(
    unsigned char *buf, size_t size, unsigned char *(*get_sp)(void))
{
    pfstest_avr_mem_usage_t used = {0, 0, false};
    unsigned char *sp = get_sp();
    unsigned char *p = buf + (size - 1);
    size_t sentinel_count = 0;
    unsigned char *found = NULL;

    if (sp < p)
        p = sp;

    assert(size > 0);
    assert(sp >= buf);

    while (1) {
        if (*p == get_fill_char(p - buf)) {
            if (sentinel_count == 0)
                found = p + 1;
            sentinel_count++;
        } else {
            sentinel_count = 0;
        }

        if (sentinel_count == 4)
            break;
        if (p == buf) {
            used.collision = true;
            return used;
        }

        p--;
    }
    used.stack = size - (size_t)(found - buf);

    while (p != buf) {
        if (*p != get_fill_char(p - buf)) {
            p++;
            break;
        }

        p--;
    }
    used.heap = (size_t)(p - buf);

    return used;
}
