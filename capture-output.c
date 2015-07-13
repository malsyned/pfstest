#include "capture-output.h"

#include <stdio.h>

char captured_output[300];
pfstest_output_formatter_t message_spy;

static int captured_output_idx = 0;

void capture_output_init(void)
{
    captured_output[0] = '\0';
    captured_output_idx = 0;
    pfstest_output_formatter_message_spy_init(&message_spy,
                                              capture_output_char);
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
