/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "capture-output.h"

#include "pfstest-alloc.h"
#include "pfstest-reporter-message-spy.h"

#include <stdio.h>

#define CAPTURED_OUTPUT_SIZE 340
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

static void register_declared_test_with_suite(pfstest_list_t *suite,
                                              pfstest_t *the_test)
{
    _pfstest_suite_register_test(suite, the_test);
}

void capture_test_results_with_plugins(pfstest_t *the_test,
                                       pfstest_list_t *plugins)
{
    pfstest_list_t *suite = pfstest_alloc(sizeof(*suite));
    pfstest_list_reset(suite);
    register_declared_test_with_suite(suite, the_test);
    pfstest_suite_run(plugins, suite, NULL, NULL, message_spy);
}
