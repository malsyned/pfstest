#include "pfstest-matcher.h"

#include "pfstest-alloc.h"

bool pfstest_matcher_matches(pfstest_matcher_t *matcher,
                             pfstest_value_t *actual)
{
    return matcher->test(matcher, actual);
}

void pfstest_matcher_print(pfstest_matcher_t *matcher)
{
    matcher->printer(matcher);
}

pfstest_matcher_t *pfstest_matcher_new(
    void (*printer)(pfstest_matcher_t *),
    bool (*test)(pfstest_matcher_t *matcher, pfstest_value_t *actual),
    void *data)
{
    pfstest_matcher_t *m = pfstest_alloc(sizeof(*m));

    m->printer = printer;
    m->test = test;
    m->data = data;

    return m;
}

void *pfstest_matcher_data(pfstest_matcher_t *matcher)
{
    return matcher->data;
}
