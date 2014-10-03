#include "pfstest-value.h"

#include "pfstest-alloc.h"

void pfstest_value_print(pfstest_value_t *value)
{
    value->printer(value);
}

pfstest_value_t *pfstest_value_new(void (*printer)(pfstest_value_t *),
                                   void *data)
{
    pfstest_value_t *v = pfstest_alloc(sizeof(*v));

    v->printer = printer;
    v->data = data;

    return v;
}

void *pfstest_value_data(pfstest_value_t *value)
{
    return value->data;
}
