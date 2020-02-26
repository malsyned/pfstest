#include "pfstest-arg-handler.h"

#include "pfstest-alloc.h"

pfstest_tag_t pfstest_arg_handler_tag = PFSTEST_TAG_AUTO;

pfstest_arg_handler_t *pfstest_arg_handler_new(
    pfstest_bool (*test_function)(pfstest_arg_handler_t *arg_handler,
                                  pfstest_value_t *actual),
    void (*matched_function)(pfstest_arg_handler_t *arg_handler,
                             pfstest_value_t *actual),
    void (*printer)(pfstest_arg_handler_t *arg_handler,
                    pfstest_reporter_t *reporter),
    void *data)
{
    pfstest_arg_handler_t *h = pfstest_alloc(sizeof(*h));
    pfstest_tagged_init((pfstest_tagged_t *)h, &pfstest_arg_handler_tag);

    h->test_function = test_function;
    h->matched_function = matched_function;
    h->printer = printer;
    h->data = data;

    return h;
}

void pfstest_arg_handler_print(pfstest_arg_handler_t *arg_handler,
                               pfstest_reporter_t *reporter)
{
    arg_handler->printer(arg_handler, reporter);
}

pfstest_bool pfstest_arg_handler_test(pfstest_arg_handler_t *arg_handler,
                                      pfstest_value_t *actual)
{
    return arg_handler->test_function(arg_handler, actual);
}

void pfstest_arg_handler_matched(pfstest_arg_handler_t *arg_handler,
                                 pfstest_value_t *actual)
{
    arg_handler->matched_function(arg_handler, actual);
}

void *pfstest_arg_handler_data(pfstest_arg_handler_t *arg_handler)
{
    return arg_handler->data;
}
