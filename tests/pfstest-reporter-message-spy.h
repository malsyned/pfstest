#ifndef PFSTEST_REPORTER_MESSAGE_SPY_H
#define PFSTEST_REPORTER_MESSAGE_SPY_H

#include "pfstest-reporter.h"

pfstest_reporter_t *pfstest_reporter_message_spy_new(int (*print_char)(int));

#endif /* !PFSTEST_REPORTER_MESSAGE_SPY_H */
