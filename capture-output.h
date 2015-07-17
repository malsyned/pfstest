#ifndef CAPTURE_OUTPUT_H
#define CAPTURE_OUTPUT_H

#include "pfstest-core.h"

extern char captured_output[];
extern pfstest_output_formatter_t message_spy;

void capture_output_init(void);
int capture_output_char(int c);
void capture_test_results(pfstest_t *the_test);

#endif /* !CAPTURE_OUTPUT_H */
