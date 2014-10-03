#include "pfstest-matcher.h"

bool pfstest_matcher_matches(pfstest_matcher_t *matcher,
                             pfstest_value_t *actual)
{
    return matcher->test(matcher, actual);
}

void pfstest_matcher_print(pfstest_matcher_t *matcher)
{
    matcher->printer(matcher);
}
