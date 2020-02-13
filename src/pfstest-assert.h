#ifndef PFSTEST_ASSERT_H
#define PFSTEST_ASSERT_H

/** @file */

#include "pfstest-value.h"
#include "pfstest-matcher.h"

void _pfstest_assert_that_at_location(const pfstest_pg_ptr char *file,
                                      int line,
                                      const pfstest_pg_ptr char *message,
                                      pfstest_value_t *actual,
                                      pfstest_matcher_t *matcher);
/** Like #pfstest_assert_that, but overriding of the file name and
 * line number reported */
#define pfstest_assert_that_at_location(file, line, message,            \
                                        actual, matcher)                \
    _pfstest_assert_that_at_location(file, line, pfstest_pg_str(message), \
                                     actual, matcher)
/** Fail the running test if @p actual doesn't pass the test described
 * by @p matcher.
 *
 * @param message: A description of the behavior being tested
 * @param actual: A pfstest_value_t containing the value produced by
 * the code-under-test.
 * @param matcher: A pfstest_matcher_t used to test whether @p actual
 * is a correct value.
 */
#define pfstest_assert_that(message, actual, matcher)                   \
    pfstest_assert_that_at_location(                                    \
        __PFSTEST_NV_FILE__, __PFSTEST_LINE__, message, actual, matcher)

/** @nonamespace_section */
/** @{ */

#ifndef PFSTEST_NOALIAS_assert_that
/** @nonamespace_alias{PFSTEST_NOALIAS_assert_that} */
# define assert_that pfstest_assert_that
#endif

/** @} */

#endif /* !PFSTEST_ASSERT_H */
