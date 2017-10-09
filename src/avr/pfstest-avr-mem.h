#ifndef PFSTEST_AVR_MEM_H
#define PFSTEST_AVR_MEM_H

#include <stddef.h>
#include <stdbool.h>

typedef struct
{
    size_t heap;
    size_t stack;
    bool collision;
} pfstest_avr_mem_usage_t;

void pfstest_avr_fill_heap_with_sentinel(void);
pfstest_avr_mem_usage_t pfstest_avr_max_heap_and_stack_usage(void);
size_t pfstest_avr_malloc_used(void);

void _pfstest_avr_fill_mem_with_sentinel(unsigned char *heap,
                                         unsigned char *(*get_sp)(void));
pfstest_avr_mem_usage_t _pfstest_avr_mem_usage(
    unsigned char *buf, size_t size, unsigned char *(*get_sp)(void));

#endif /* !defined(PFSTEST_AVR_MEM_H) */
