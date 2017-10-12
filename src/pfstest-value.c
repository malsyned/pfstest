#include "pfstest-value.h"

#include "pfstest-alloc.h"

void pfstest_value_print(pfstest_reporter_t *reporter, pfstest_value_t *value)
{
    value->printer(reporter, value);
}

pfstest_value_t *pfstest_value_new(
    void (*printer)(pfstest_reporter_t *reporter, pfstest_value_t *),
    const void *data, size_t size)
{
    return pfstest_value_new_with_aux(printer, data, size, NULL);
}

pfstest_value_t *pfstest_value_new_with_aux(
    void (*printer)(pfstest_reporter_t *reporter, pfstest_value_t *),
    const void *data, size_t size, void *aux)
{
    pfstest_value_t *v = pfstest_alloc(sizeof(*v));

    v->printer = printer;
    v->data = data;
    v->size = size;
    v->aux = aux;

    return v;
}

const void *pfstest_value_data(pfstest_value_t *value)
{
    return value->data;
}

size_t pfstest_value_size(pfstest_value_t *value)
{
    return value->size;
}

void *pfstest_value_aux(pfstest_value_t *value)
{
    return value->aux;
}
