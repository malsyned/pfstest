#include "capture-output.h"

#include "pfstest-alloc.h"
#include "pfstest-reporter-message-spy.h"

#include <stdio.h>

#define CAPTURED_OUTPUT_SIZE 320
char *captured_output;
pfstest_reporter_t *message_spy;

static unsigned int captured_output_idx = 0;

void capture_output_init(void)
{
    captured_output = pfstest_alloc(CAPTURED_OUTPUT_SIZE);
    captured_output[0] = '\0';
    captured_output_idx = 0;
    message_spy = pfstest_reporter_message_spy_new(capture_output_char);
}

int capture_output_char(int c)
{
    if (captured_output_idx < CAPTURED_OUTPUT_SIZE - 1) {
        captured_output[captured_output_idx++] = (char)c;
        captured_output[captured_output_idx] = '\0';
        return c;
    }

    return EOF;
}

void capture_test_results(pfstest_t *the_test)
{
    capture_test_results_with_plugins(the_test, NULL);
}

void capture_test_results_with_plugins(pfstest_t *the_test,
                                       pfstest_list_t *plugins)
{
    pfstest_run(the_test, NULL, NULL, plugins, message_spy);
}
