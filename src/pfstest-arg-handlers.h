#ifndef PFSTEST_ARG_HANDLERS_H
#define PFSTEST_ARG_HANDLERS_H

#include "pfstest-arg-handler.h"
#include "pfstest-matcher.h"

pfstest_arg_handler_t *pfstest_arg_that(pfstest_matcher_t *matcher);
pfstest_arg_handler_t *pfstest_assign_arg(pfstest_value_t *src);
pfstest_arg_handler_t *pfstest_assign_arg_that(pfstest_matcher_t *matcher,
                                               pfstest_value_t *src);
pfstest_arg_handler_t *pfstest_capture_arg(void *arg_p);

#ifndef PFSTEST_NOALIAS_arg_that
# define arg_that pfstest_arg_that
#endif
#ifndef PFSTEST_NOALIAS_assign_arg
# define assign_arg pfstest_assign_arg
#endif
#ifndef PFSTEST_NOALIAS_assign_arg_that
# define assign_arg_that pfstest_assign_arg_that
#endif
#ifndef PFSTEST_NOALIAS_capture_arg
# define capture_arg pfstest_capture_arg
#endif

#endif /* !PFSTEST_ARG_HANDLERS_H */
