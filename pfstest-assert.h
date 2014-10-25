#ifndef PFSTEST_ASSERT_H
#define PFSTEST_ASSERT_H

#include "pfstest-value.h"
#include "pfstest-matcher.h"

void _pfstest_assert_that_at_location(pfstest_nv_str_ptr(file),
                                      int line,
                                      pfstest_nv_str_ptr(message),
                                      pfstest_value_t *actual,
                                      pfstest_matcher_t *matcher);
#define pfstest_assert_that_at_location(file, line, message,            \
                                        actual, matcher)                \
    _pfstest_assert_that_at_location(file, line, pfstest_nv_string(message), \
                                     actual, matcher)
#define pfstest_assert_that(message, actual, matcher)   \
    pfstest_assert_that_at_location(                    \
        pfstest_nv_string(__FILE__), __LINE__, message, actual, matcher)

#ifndef PFSTEST_NOALIAS_assert_that
# define assert_that pfstest_assert_that
#endif

#endif /* !PFSTEST_ASSERT_H */
