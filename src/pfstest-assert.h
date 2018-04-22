#ifndef PFSTEST_ASSERT_H
#define PFSTEST_ASSERT_H

#include "pfstest-value.h"
#include "pfstest-matcher.h"

void _pfstest_assert_that_at_location(const pfstest_pg_ptr char *file,
                                      int line,
                                      const pfstest_pg_ptr char *message,
                                      pfstest_value_t *actual,
                                      pfstest_matcher_t *matcher);
#define pfstest_assert_that_at_location(file, line, message,            \
                                        actual, matcher)                \
    _pfstest_assert_that_at_location(file, line, pfstest_pg_str(message), \
                                     actual, matcher)
#define pfstest_assert_that(message, actual, matcher)                   \
    pfstest_assert_that_at_location(                                    \
        __PFSTEST_NV_FILE__, __PFSTEST_LINE__, message, actual, matcher)

#ifndef PFSTEST_NOALIAS_assert_that
# define assert_that pfstest_assert_that
#endif

#endif /* !PFSTEST_ASSERT_H */
