/* SPDX-License-Identifier: LGPL-2.1-or-later */

#ifndef PFSTEST_ARG_HANDLERS_H
#define PFSTEST_ARG_HANDLERS_H

/** @file
 * @c arg_handlers work with mock expectations to describe how
 * mocks should relate to the arguments that are passed to them by the
 * code-under-test. */

#include "pfstest-arg-handler.h"
#include "pfstest-matcher.h"

/** Allow an expectation to match an invocation if the invocation's
 * argument passes @c matcher */
pfstest_arg_handler_t *pfstest_arg_that(pfstest_matcher_t *matcher);
/** If an invocation otherwise matches an expectation, copy the data
 * from @c src onto the memory pointed to by this argument */
pfstest_arg_handler_t *pfstest_assign_arg(pfstest_value_t *src);
/** Combines the behaviors of #arg_that and #assign_arg */
pfstest_arg_handler_t *pfstest_assign_arg_that(pfstest_matcher_t *matcher,
                                               pfstest_value_t *src);
/** If an invocation otherwise matches an expectation, copy the
 * argument passed to the code-under-test onto the memory pointed to
 * by @c arg_p */
pfstest_arg_handler_t *pfstest_capture_arg(void *arg_p);

/** @nonamespace_section */
/** @{ */

#ifndef PFSTEST_NOALIAS_arg_that
/** @nonamespace_alias{PFSTEST_NOALIAS_arg_that} */
# define arg_that pfstest_arg_that
#endif
#ifndef PFSTEST_NOALIAS_assign_arg
/** @nonamespace_alias{PFSTEST_NOALIAS_assign_arg} */
# define assign_arg pfstest_assign_arg
#endif
#ifndef PFSTEST_NOALIAS_assign_arg_that
/** @nonamespace_alias{PFSTEST_NOALIAS_assign_arg_that} */
# define assign_arg_that pfstest_assign_arg_that
#endif
#ifndef PFSTEST_NOALIAS_capture_arg
/** @nonamespace_alias{PFSTEST_NOALIAS_capture_arg} */
# define capture_arg pfstest_capture_arg
#endif

/** @} */

#endif /* !PFSTEST_ARG_HANDLERS_H */
