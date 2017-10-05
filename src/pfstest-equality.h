#ifndef PFSTEST_EQUALITY_H
#define PFSTEST_EQUALITY_H

#include "pfstest-matcher.h"
#include "pfstest-value.h"

void pfstest_equality_printer(pfstest_reporter_t *reporter,
                              pfstest_matcher_t *matcher);
bool pfstest_equality_test(pfstest_matcher_t *matcher,
                           pfstest_value_t *actual_value);

#endif /* !defined(PFSTEST_EQUALITY_H) */
