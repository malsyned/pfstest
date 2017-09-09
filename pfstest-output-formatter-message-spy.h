#ifndef PFSTEST_OUTPUT_FORMATTER_MESSAGE_SPY_H
#define PFSTEST_OUTPUT_FORMATTER_MESSAGE_SPY_H

#include "pfstest-output.h"

pfstest_output_formatter_t *pfstest_output_formatter_message_spy_new(
    int (*print_char)(int));

#endif /* !PFSTEST_OUTPUT_FORMATTER_MESSAGE_SPY_H */
