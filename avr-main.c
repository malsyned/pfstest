#include <stdio.h>

#define __ASSERT_USE_STDERR
#include <assert.h>

#include "pfstest.h"

/*
 *  This port correponds to the "-W 0x20,-" command line option.
 */
#define special_output_port (*( (volatile char *)0x20))

static int dbg_putchar(char c, FILE *stream)
{
    special_output_port = c;
    return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(dbg_putchar, NULL, _FDEV_SETUP_WRITE);

extern char *__brkval;
extern char *__malloc_heap_start;

size_t malloc_used(void)
{
    if (__brkval == 0)          /* Uninitialized case */
        return 0;
    else
        return (size_t)(__brkval - __malloc_heap_start);
}

int main(void)
{
    stdout = &mystdout;

    int r = run_all_tests_verbose();
    
    assert(malloc_used() == 0);

    return r;
}
