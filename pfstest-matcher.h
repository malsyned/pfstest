#ifndef PFSTEST_MATCHER_H
#define PFSTEST_MATCHER_H

#include <stdbool.h>

#include "pfstest-value.h"

typedef struct _pfstest_matcher_t pfstest_matcher_t;

struct _pfstest_matcher_t
{
    void (*printer)(pfstest_matcher_t *);
    bool (*test)(pfstest_matcher_t *matcher, pfstest_value_t *actual);
    void *data;
};

pfstest_matcher_t *pfstest_matcher_new(
    void (*printer)(pfstest_matcher_t *),
    bool (*test)(pfstest_matcher_t *matcher, pfstest_value_t *actual),
    void *data);
bool pfstest_matcher_matches(pfstest_matcher_t *matcher,
                             pfstest_value_t *actual);
void pfstest_matcher_print(pfstest_matcher_t *matcher);
void *pfstest_matcher_data(pfstest_matcher_t *matcher);

#endif /* !PFSTEST_MATCHER_H */
