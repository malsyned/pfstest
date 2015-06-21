#ifndef PFSTEST_VALUE_H
#define PFSTEST_VALUE_H

#include <stddef.h>
#include "pfstest-output.h"

typedef struct _pfstest_value_t pfstest_value_t;

struct _pfstest_value_t
{
    void (*printer)(pfstest_output_formatter_t *formatter, pfstest_value_t *);
    void *data;
    size_t size;
};

void pfstest_value_print(pfstest_output_formatter_t *formatter,
                         pfstest_value_t *value);
pfstest_value_t *pfstest_value_new(
    void (*printer)(pfstest_output_formatter_t *formatter, pfstest_value_t *),
    void *data, size_t size);
void *pfstest_value_data(pfstest_value_t *value);
size_t pfstest_value_size(pfstest_value_t *value);

#endif /* !PFSTEST_VALUE_H */
