#ifndef PFSTEST_ASSERT_H
#define PFSTEST_ASSERT_H

#include "pfstest-value.h"
#include "pfstest-matcher.h"

void _pfstest_assert_that(const char *file,
                          int line,
                          const char *message,
                          pfstest_value_t *actual,
                          pfstest_matcher_t *matcher);
#define pfstest_assert_that(message, actual, matcher)                   \
    _pfstest_assert_that(__FILE__, __LINE__, message, actual, matcher)

#ifndef PFSTEST_NOALIAS_ASSERT_THAT
# define assert_that pfstest_assert_that
#endif

#endif /* !PFSTEST_ASSERT_H */
