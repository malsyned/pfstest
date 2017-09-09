#include "capture-output.h"

#include "pfstest-output-formatter-message-spy.h"

#include <stdio.h>

char captured_output[310];
pfstest_output_formatter_t *message_spy;

static unsigned int captured_output_idx = 0;

void capture_output_init(void)
{
    captured_output[0] = '\0';
    captured_output_idx = 0;
    message_spy =
        pfstest_output_formatter_message_spy_new(capture_output_char);
}

int capture_output_char(int c)
{
    if (captured_output_idx < sizeof(captured_output) - 1) {
        captured_output[captured_output_idx++] = c;
        captured_output[captured_output_idx] = '\0';
        return c;
    }

    return EOF;
}

void capture_test_results(pfstest_t *the_test)
{
    pfstest_run(the_test, NULL, NULL, NULL, NULL, message_spy);
}
