#include "pfstest-value.h"

#include "pfstest-alloc.h"

pfstest_tag_t pfstest_value_tag = PFSTEST_TAG_AUTO;

void pfstest_value_print(pfstest_value_t *value, pfstest_reporter_t *reporter)
{
    value->printer(value, reporter);
}

void pfstest_value_init(
    pfstest_value_t *value,
    void (*printer)(pfstest_value_t *value, pfstest_reporter_t *reporter),
    const void *data, size_t size)
{
    pfstest_tagged_init((pfstest_tagged_t *)value, &pfstest_value_tag);

    value->printer = printer;
    value->data = data;
    value->size = size;
}

pfstest_value_t *pfstest_value_new(
    void (*printer)(pfstest_value_t *value, pfstest_reporter_t *reporter),
    const void *data, size_t size)
{
    pfstest_value_t *v = pfstest_alloc(sizeof(*v));
    pfstest_value_init(v, printer, data, size);

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
