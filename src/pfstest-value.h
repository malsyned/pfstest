#ifndef PFSTEST_VALUE_H
#define PFSTEST_VALUE_H

#include <stddef.h>
#include "pfstest-reporter.h"

typedef struct _pfstest_value_t pfstest_value_t;

struct _pfstest_value_t
{
    void (*printer)(pfstest_reporter_t *reporter, pfstest_value_t *);
    const void *data;           /* Boxed version of the data value */
    size_t size;    /* Size of boxed object, or 0 for boxed pointer */
    void *aux;                  /* Additional free-form information */
};

void pfstest_value_print(pfstest_reporter_t *reporter,
                         pfstest_value_t *value);
pfstest_value_t *pfstest_value_new(
    void (*printer)(pfstest_reporter_t *reporter, pfstest_value_t *),
    const void *data, size_t size, void *aux);
const void *pfstest_value_data(pfstest_value_t *value);
size_t pfstest_value_size(pfstest_value_t *value);
void *pfstest_value_aux(pfstest_value_t *value);

#endif /* !PFSTEST_VALUE_H */
