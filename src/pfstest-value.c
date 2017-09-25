#include "pfstest-value.h"

#include "pfstest-alloc.h"

void pfstest_value_print(pfstest_output_formatter_t *formatter,
                         pfstest_value_t *value)
{
    value->printer(formatter, value);
}

pfstest_value_t *pfstest_value_new(
    void (*printer)(pfstest_output_formatter_t *formatter, pfstest_value_t *),
    void *data, size_t size)
{
    pfstest_value_t *v = pfstest_alloc(sizeof(*v));

    v->printer = printer;
    v->data = data;
    v->size = size;

    return v;
}

void *pfstest_value_data(pfstest_value_t *value)
{
    return value->data;
}

size_t pfstest_value_size(pfstest_value_t *value)
{
    return value->size;
}
