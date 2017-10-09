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

void pfstest_avr_reset_stack_high_water_mark(void)
{
    size_t heap_size =
        (size_t)((unsigned char *)(RAMEND + 1) - &__heap_start);
    _pfstest_avr_fill_unused_stack(&__heap_start, heap_size,
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

static void fill_to_sp(unsigned char *buf, unsigned char *start,
                       unsigned char *(*get_sp)(void))
{
    size_t i = (size_t)(start - buf);
    unsigned char *sp = get_sp();
    unsigned char *p = start;

    while (p <= sp) {
        *p++ = get_fill_char(i++);
    }
}

void _pfstest_avr_fill_mem_with_sentinel(unsigned char *heap,
                                         unsigned char *(*get_sp)(void))
{
    fill_to_sp(heap, heap, get_sp);
}

static unsigned char *find_stack_high_water_mark(unsigned char *buf,
                                                 unsigned char *start)
{
    unsigned char *p = start;
    size_t sentinel_count = 0;
    unsigned char *found = NULL;

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
            return NULL;
        }

        p--;
    }

    return found;
}

static unsigned char *find_heap_high_water_mark(unsigned char *buf,
                                                unsigned char *highest_sp)
{
    unsigned char *p = highest_sp;

    while (p != buf) {
        if (*p != get_fill_char(p - buf)) {
            p++;
            break;
        }

        p--;
    }

    return p;
}

pfstest_avr_mem_usage_t _pfstest_avr_mem_usage(
    unsigned char *buf, size_t size, unsigned char *(*get_sp)(void))
{
    pfstest_avr_mem_usage_t used = {0, 0, false};
    unsigned char *sp = get_sp();
    unsigned char *stack_hwm, *heap_hwm;

    assert(size > 0);
    assert(sp >= buf);
    assert(sp < buf + size);

    stack_hwm = find_stack_high_water_mark(buf, sp);
    if (stack_hwm == NULL) {
        used.collision = true;
        return used;
    }
    used.stack = (size_t)(buf + size - stack_hwm);

    heap_hwm = find_heap_high_water_mark(buf, stack_hwm - 1);
    used.heap = (size_t)(heap_hwm - buf);

    return used;
}

void _pfstest_avr_fill_unused_stack(unsigned char *buf, size_t size,
                                    unsigned char *(*get_sp)(void))
{
    unsigned char *sp = get_sp();
    unsigned char *stack_hwm;

    assert(size > 0);
    assert(sp >= buf);
    assert(sp < buf + size);

    stack_hwm = find_stack_high_water_mark(buf, sp);
    fill_to_sp(buf, stack_hwm, get_sp);
}
