#include <stdio.h>

#include <assert.h>
#include <string.h>

#include "pfstest.h"

#include "pfstest-avr-mem.h"

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

static int print_char(int c)
{
    return putchar(c);
}

static void print_heap_and_stack_usage(pfstest_avr_mem_usage_t *usage)
{
    if (usage->collision) {
        printf("FATAL: Stack and heap collided!\n");
    } else {
        printf("Max stack used: %u; Max heap used: %u\n",
               usage->stack, usage->heap);
    }
}

int main(void)
{
    pfstest_avr_mem_usage_t usage;
    pfstest_reporter_t *reporter;
    int r;

    pfstest_avr_fill_heap_with_sentinel();

    stdout = &mystdout;
    stderr = stdout;

    reporter = pfstest_reporter_verbose_new(
        print_char, pfstest_report_colorizer_null);

    r = pfstest_run_registered_tests(NULL, NULL, reporter);
    
    pfstest_alloc_free_frame();
    assert(pfstest_avr_malloc_used() == 0);

    usage = pfstest_avr_max_heap_and_stack_usage();
    assert(!usage.collision);
    print_heap_and_stack_usage(&usage);

    return r;
}
