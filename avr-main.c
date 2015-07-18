#include <stdio.h>

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

static int print_char(int c)
{
    return putchar(c);
}

int main(void)
{
    pfstest_arguments_t args = {};
    args.verbose = true;

    stdout = &mystdout;
    stderr = stdout;

    int r = pfstest_start(print_char, &args);
    
    assert(malloc_used() == 0);

    return r;
}
