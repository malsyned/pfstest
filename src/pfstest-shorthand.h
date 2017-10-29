#ifndef PFSTEST_SHORTHAND_H
#define PFSTEST_SHORTHAND_H

#include <stddef.h>

#include "pfstest-assert.h"
#include "pfstest-values.h"
#include "pfstest-matchers.h"

#define pfstest_assert_true(message, value)                             \
    pfstest_assert_that(message, the_bool(value), is(the_bool(pfstest_true)))
#define pfstest_assert_false(message, value)                            \
    pfstest_assert_that(message, the_bool(value), is(the_bool(pfstest_false)))
#define pfstest_assert_null(message, value)                             \
    pfstest_assert_that(message, the_pointer(value), is(the_pointer(NULL)))

#ifndef PFSTEST_NOALIAS_assert_true
# define assert_true pfstest_assert_true
#endif
#ifndef PFSTEST_NOALIAS_assert_false
# define assert_false pfstest_assert_false
#endif
#ifndef PFSTEST_NOALIAS_assert_null
# define assert_null pfstest_assert_null
#endif

#endif /* !PFSTEST_SHORTHAND_H */
