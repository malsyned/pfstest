/* SPDX-License-Identifier: LGPL-2.1-or-later */

#ifndef CAPTURE_OUTPUT_H
#define CAPTURE_OUTPUT_H

#include "pfstest-core.h"

extern char *captured_output;
extern pfstest_reporter_t *message_spy;

void capture_output_init(void);
int capture_output_char(int c);
void capture_test_results(pfstest_t *the_test);
void capture_test_results_with_plugins(pfstest_t *the_test,
                                       pfstest_list_t *plugins);

#endif /* !CAPTURE_OUTPUT_H */
