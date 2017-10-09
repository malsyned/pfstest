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

static size_t max_stack_used = 0;

static void reset_stack_usage(void)
{
    pfstest_avr_mem_usage_t usage = pfstest_avr_max_heap_and_stack_usage();
    if (usage.stack > max_stack_used)
        max_stack_used = usage.stack;
    pfstest_avr_reset_stack_high_water_mark();
}

static void print_memory_usage(void)
{
    pfstest_avr_mem_usage_t usage = pfstest_avr_max_heap_and_stack_usage();
    if (usage.collision) {
        printf("FATAL: Stack and heap collided!\n");
    } else {
        printf("Stack used: %u; Heap used: %d\n",
               (unsigned)usage.stack, (unsigned)pfstest_avr_malloc_used());
    }
}

static void print_max_memory_usage(void)
{
    pfstest_avr_mem_usage_t usage = pfstest_avr_max_heap_and_stack_usage();

    if (usage.collision) {
        printf("FATAL: Stack and heap collided!\n");
    } else {
        printf("Max stack used: %u; Max heap used: %u\n",
               max_stack_used, usage.heap);
    }
}

pfstest_plugin_define(memory_usage_plugin,
                      reset_stack_usage,
                      NULL,
                      print_memory_usage);

int main(void)
{
    pfstest_reporter_t *reporter;
    int r;

    pfstest_avr_fill_heap_with_sentinel();

    /* Uncomment this line to add a stack and heap report after every
     * test. */
    /* pfstest_register_plugin(memory_usage_plugin); */

    stdout = &mystdout;
    stderr = stdout;

    reporter = pfstest_reporter_verbose_new(
        print_char, pfstest_report_colorizer_null);

    r = pfstest_run_registered_tests(NULL, NULL, reporter);
    
    pfstest_alloc_free_frame();
    assert(pfstest_avr_malloc_used() == 0);

    print_max_memory_usage();

    return r;
}
