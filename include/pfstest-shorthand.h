/* SPDX-License-Identifier: LGPL-2.1-or-later */

#ifndef PFSTEST_SHORTHAND_H
#define PFSTEST_SHORTHAND_H

/** @file */

#include <stddef.h>

#include "pfstest-assert.h"
#include "pfstest-values.h"
#include "pfstest-matchers.h"

#define pfstest_assert_true(message, value)                             \
    pfstest_assert_that(message, pfstest_the_bool(value),               \
                        pfstest_equal_to(pfstest_the_bool(pfstest_true)))
#define pfstest_assert_false(message, value)                            \
    pfstest_assert_that(message, pfstest_the_bool(value),               \
                        pfstest_equal_to(pfstest_the_bool(pfstest_false)))
#define pfstest_assert_null(message, value)                             \
    pfstest_assert_that(message, pfstest_the_pointer(value),            \
                        pfstest_equal_to(pfstest_the_pointer(NULL)))

/** @nonamespace_section */
/** @{ */

#ifndef PFSTEST_NOALIAS_assert_true
/** @nonamespace_alias{PFSTEST_NOALIAS_assert_true} */
# define assert_true pfstest_assert_true
#endif
#ifndef PFSTEST_NOALIAS_assert_false
/** @nonamespace_alias{PFSTEST_NOALIAS_assert_false} */
# define assert_false pfstest_assert_false
#endif
#ifndef PFSTEST_NOALIAS_assert_null
/** @nonamespace_alias{PFSTEST_NOALIAS_assert_null} */
# define assert_null pfstest_assert_null
#endif

/** @} */

#endif /* !PFSTEST_SHORTHAND_H */
