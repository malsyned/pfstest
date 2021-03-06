/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest-matcher.h"

#include "pfstest-alloc.h"

pfstest_tag_t pfstest_matcher_tag = PFSTEST_TAG_AUTO;

pfstest_bool pfstest_matcher_matches(pfstest_matcher_t *matcher,
                                     pfstest_value_t *actual)
{
    return matcher->test(matcher, actual);
}

void pfstest_matcher_print(pfstest_matcher_t *matcher,
                           pfstest_reporter_t *reporter)
{
    matcher->printer(matcher, reporter);
}

pfstest_matcher_t *pfstest_matcher_new(
    void (*printer)(pfstest_matcher_t *matcher, pfstest_reporter_t *reporter),
    pfstest_bool (*test)(pfstest_matcher_t *matcher, pfstest_value_t *actual),
    void *data)
{
    pfstest_matcher_t *m = pfstest_alloc(sizeof(*m));
    pfstest_tagged_init((pfstest_tagged_t *)m, &pfstest_matcher_tag);

    m->printer = printer;
    m->test = test;
    m->data = data;

    return m;
}

void *pfstest_matcher_data(pfstest_matcher_t *matcher)
{
    return matcher->data;
}
