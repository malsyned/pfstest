/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest-value.h"

#include "pfstest-alloc.h"

pfstest_tag_t pfstest_value_tag = PFSTEST_TAG_AUTO;

void pfstest_value_print(pfstest_value_t *value, pfstest_reporter_t *reporter)
{
    value->printer(value, reporter);
}

pfstest_value_t *pfstest_value_new(
    void (*printer)(pfstest_value_t *value, pfstest_reporter_t *reporter),
    const void *data, size_t size, void *aux)
{
    pfstest_value_t *v = pfstest_alloc(sizeof(*v));
    pfstest_tagged_init((pfstest_tagged_t *)v, &pfstest_value_tag);

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
