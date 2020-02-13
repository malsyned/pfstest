#ifndef PFSTEST_MATCHERS_H
#define PFSTEST_MATCHERS_H

/** @file */

#include <stddef.h>

#include "pfstest-platform.h"
#include "pfstest-matcher.h"
#include "pfstest-value.h"

/** Perform a byte-by-byte comparison to @p value */
pfstest_matcher_t *pfstest_is(pfstest_value_t *value);

/** Test whether a string compares equal to @p s as if by strcmp. On
 * Harvard architectures, @p s is expected to be stored in the Program
 * Memory space */
pfstest_matcher_t *pfstest_matches_the_pg_string(
    const pfstest_pg_ptr char *s);

/** Test each member of an @c int array as boxed by #the_int_array
 * with a matcher from the NULL-terminated argument list */
pfstest_matcher_t *pfstest_int_members_match(pfstest_matcher_t *first, ...);

pfstest_matcher_t *_pfstest_is_anything(void);
/** This matcher matches any value (useful for disregarding arguments
 * in mocks.) */
#define pfstest_is_anything (_pfstest_is_anything())

#ifndef PFSTEST_NOALIAS_is
/** @nonamespace_alias{PFSTEST_NOALIAS_is} */
# define is pfstest_is
#endif

#ifndef PFSTEST_NOALIAS_matches_the_pg_string
/** @nonamespace_alias{PFSTEST_NOALIAS_matches_the_pg_string} */
# define matches_the_pg_string pfstest_matches_the_pg_string
#endif
#ifndef PFSTEST_NOALIAS_int_members_match
/** @nonamespace_alias{PFSTEST_NOALIAS_int_members_match} */
# define int_members_match pfstest_int_members_match
#endif
#ifndef PFSTEST_NOALIAS_is_anything
/** @nonamespace_alias{PFSTEST_NOALIAS_is_anything} */
# define is_anything pfstest_is_anything
#endif

#endif /* !PFSTEST_MATCHERS_H */
