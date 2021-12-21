/* SPDX-License-Identifier: LGPL-2.1-or-later */

#ifndef PFSTEST_MATCHERS_H
#define PFSTEST_MATCHERS_H

/** @file */

#include <stddef.h>

#include "pfstest-platform.h"
#include "pfstest-matcher.h"
#include "pfstest-value.h"

/** @name Matchers */
/** @{ */

/** Return a #pfstest_matcher_t argument unchanged, wrap a
 * #pfstest_value_t argument in equal_to(). Intended to make tests
 * more readable. */
#define pfstest_is(x) pfstest_is_at_location(__PFSTEST_NV_FILE__,   \
                                             __PFSTEST_LINE__, x)
/** @cond pfstest_at_location */
pfstest_matcher_t *pfstest_is_at_location(const pfstest_pg_ptr char *file,
                                          int line,
                                          void *matcher_or_value);
/** @endcond */

/** Perform a byte-by-byte comparison to @p value */
pfstest_matcher_t *pfstest_equal_to(pfstest_value_t *value);

/** Test whether a string compares equal to @p s as if by strcmp. On
 * Harvard architectures, @p s is expected to be stored in the Program
 * Memory space */
pfstest_matcher_t *pfstest_matches_the_pg_string(
    const pfstest_pg_ptr char *s);

/** Test each member of an @c int array as boxed by #the_int_array
 * with a matcher from the NULL-terminated argument list */
pfstest_matcher_t *pfstest_int_members_match(pfstest_matcher_t *first, ...);

/** Test each member of an @c unsigned @c int array as boxed by #the_uint_array
 * with a matcher from the NULL-terminated argument list */
pfstest_matcher_t *pfstest_uint_members_match(pfstest_matcher_t *first, ...);

/** This matcher matches any value (useful for disregarding arguments
 * in mocks.) */
pfstest_matcher_t *pfstest_anything(void);

/** @} */

/** @nonamespace_section */
/** @{ */

#ifndef PFSTEST_NOALIAS_is
/** @nonamespace_alias{PFSTEST_NOALIAS_is} */
# define is pfstest_is
#endif
#ifndef PFSTEST_NOALIAS_equal_to
/** @nonamespace_alias{PFSTEST_NOALIAS_equal_to} */
# define equal_to pfstest_equal_to
#endif

#ifndef PFSTEST_NOALIAS_matches_the_pg_string
/** @nonamespace_alias{PFSTEST_NOALIAS_matches_the_pg_string} */
# define matches_the_pg_string pfstest_matches_the_pg_string
#endif
#ifndef PFSTEST_NOALIAS_int_members_match
/** @nonamespace_alias{PFSTEST_NOALIAS_int_members_match} */
# define int_members_match pfstest_int_members_match
#endif
#ifndef PFSTEST_NOALIAS_uint_members_match
/** @nonamespace_alias{PFSTEST_NOALIAS_uint_members_match} */
# define uint_members_match pfstest_uint_members_match
#endif
#ifndef PFSTEST_NOALIAS_anything
/** @nonamespace_alias{PFSTEST_NOALIAS_anything} */
# define anything pfstest_anything
#endif

/** @} */

#endif /* !PFSTEST_MATCHERS_H */
