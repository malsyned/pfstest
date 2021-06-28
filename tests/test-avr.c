/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest.h"

#include <string.h>

#include "pfstest-avr-mem.h"

static unsigned char *stack_pointer;
static unsigned char *buf;
static pfstest_avr_mem_usage_t used;

setup()
{
    buf = pfstest_alloc(8);
    memset(buf, 0xa5, 8);
    stack_pointer = NULL;
}

static unsigned char *fake_get_stack_pointer(void)
{
    return stack_pointer;
}

test(should_fill_whole_buffer)
{
    static unsigned char expected[] =
        {'E', 'M', 'T', 'Y', 'E', 'M', 'T', 'Y'};

    stack_pointer = buf + 7;
    _pfstest_avr_fill_mem_with_sentinel(buf, fake_get_stack_pointer);

    assert_that("All of memory can be filled",
                the_memory(buf, 8), equal_to(the_memory(expected, 8)));
}

test(should_stop_filling_at_stack_pointer)
{
    static unsigned char expected[] =
        {'E', 'M', 'T', 'Y', 'E', 'M', 0xa5, 0xa5};

    stack_pointer = buf + 5;
    _pfstest_avr_fill_mem_with_sentinel(buf, fake_get_stack_pointer);

    assert_that("Filling stops at the stack pointer",
                the_memory(buf, 8), equal_to(the_memory(expected, 8)));
}

test(should_find_no_stack_or_heap_usage)
{
    static unsigned char mem[] =
        {'E', 'M', 'T', 'Y', 'E', 'M', 'T', 'Y'};
    stack_pointer = mem + (sizeof(mem) - 1);

    used = _pfstest_avr_mem_usage(mem, sizeof(mem), fake_get_stack_pointer);

    assert_that("No heap was used",
                the_uint((unsigned)used.heap), equal_to(the_uint(0)));
    assert_that("No stack was used",
                the_uint((unsigned)used.stack), equal_to(the_uint(0)));
    assert_false("No collision was detected", used.collision);
}

test(should_find_heap_usage)
{
    static unsigned char mem[] =
        {0xa5, 0xa5, 'T', 'Y', 'E', 'M', 'T', 'Y'};
    stack_pointer = mem + (sizeof(mem) - 1);

    used = _pfstest_avr_mem_usage(mem, sizeof(mem), fake_get_stack_pointer);

    assert_that("Some heap was used",
                the_uint((unsigned)used.heap), equal_to(the_uint(2)));
    assert_that("No stack was used",
                the_uint((unsigned)used.stack), equal_to(the_uint(0)));
    assert_false("No collision was detected", used.collision);
}

test(should_find_stack_usage)
{
    static unsigned char mem[] =
        {'E', 'M', 'T', 'Y', 'E', 0xa5, 0xa5, 0xa5};
    stack_pointer = mem + (sizeof(mem) - 1);

    used = _pfstest_avr_mem_usage(mem, sizeof(mem), fake_get_stack_pointer);

    assert_that("No heap was used",
                the_uint((unsigned)used.heap), equal_to(the_uint(0)));
    assert_that("Some stack was used",
                the_uint((unsigned)used.stack), equal_to(the_uint(3)));
    assert_false("No collision was detected", used.collision);
}

test(should_require_four_sentinels_in_stack)
{
    static unsigned char mem[] =
        {0xa5, 'M', 'T', 'Y', 'E', 0xa5, 'T', 'Y', 'E', 0xa5};
    stack_pointer = mem + (sizeof(mem) - 1);

    used = _pfstest_avr_mem_usage(mem, sizeof(mem), fake_get_stack_pointer);

    assert_that("Stack detection requires four contiguous sentinels",
                the_uint((unsigned)used.stack), equal_to(the_uint(5)));
    assert_false("No collision was detected", used.collision);
}

test(should_ignore_excess_sentinels_in_heap)
{
    static unsigned char mem[] =
        {0xa5, 'M', 'T', 'Y', 'E', 0xa5, 'T', 'Y', 'E', 'M', 0xa5};
    stack_pointer = mem + (sizeof(mem) - 1);

    used = _pfstest_avr_mem_usage(mem, sizeof(mem), fake_get_stack_pointer);

    assert_that("Sentinels in the heap are ignored",
                the_uint((unsigned)used.heap), equal_to(the_uint(6)));
    assert_false("No collision was detected", used.collision);
}

test(should_identify_collisions)
{
    static unsigned char mem[] =
        {0xa5, 0xa5, 0xa5, 0xa5, 0xa5};
    stack_pointer = mem + (sizeof(mem) - 1);

    used = _pfstest_avr_mem_usage(mem, sizeof(mem), fake_get_stack_pointer);

    assert_true("Collisions are detected", used.collision);
}

test(should_respect_stack_pointer)
{
    static unsigned char mem[] =
    {'E', 0xa5, 'T', 'Y', 'E', 'M', 0xa5, 0xa5, 'E', 'M', 'T', 'Y'};
    stack_pointer = mem + 7;

    used = _pfstest_avr_mem_usage(mem, sizeof(mem), fake_get_stack_pointer);

    assert_that("Bytes behind the stack pointer are assumed to be used",
                the_uint((unsigned)used.stack), equal_to(the_uint(6)));
}

test(should_clear_above_stack)
{
    static unsigned char mem[] =
        {'E', 'M', 'T', 'Y', 'E', 'M', 0xa5, 0xa5, 0xa5, 0xa5, 0xa5, 0xa5};
    static unsigned char expected[] =
        {'E', 'M', 'T', 'Y', 'E', 'M', 'T',  'Y',  'E',  0xa5, 0xa5, 0xa5};
    stack_pointer = mem + 8;

    _pfstest_avr_fill_unused_stack(mem, sizeof(mem), fake_get_stack_pointer);

    assert_that("Previously-used stack bytes are reinitialized",
                the_memory(mem, sizeof(mem)),
                equal_to(the_memory(expected, sizeof(expected))));
}
