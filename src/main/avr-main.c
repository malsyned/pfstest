#include <stdio.h>

#include <assert.h>
#include <string.h>

#include "pfstest.h"

/*
 *  This port correponds to the "-W 0x20,-" command line option.
 */
#define special_output_port (*( (volatile char *)0x20))

static int dbg_putchar(char c, FILE *stream)
{
    (void)stream;
    special_output_port = c;
    return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(dbg_putchar, NULL, _FDEV_SETUP_WRITE);

extern char *__brkval;
extern char *__malloc_heap_start;

static size_t malloc_used(void)
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
    pfstest_reporter_t *reporter;
    int r;

    stdout = &mystdout;
    stderr = stdout;

    reporter = pfstest_reporter_verbose_new(
        print_char, pfstest_report_colorizer_null);

    r = pfstest_run_registered_tests(NULL, NULL, reporter);
    
    pfstest_alloc_free_frame();
    assert(malloc_used() == 0);

    return r;
}
