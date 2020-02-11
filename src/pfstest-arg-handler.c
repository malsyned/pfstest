#include "pfstest-arg-handler.h"

#include "pfstest-alloc.h"

pfstest_tag_t pfstest_arg_handler_tag = PFSTEST_TAG_AUTO;

void pfstest_arg_handler_init(
    pfstest_arg_handler_t *arg_handler,
    pfstest_bool (*test_function)(pfstest_arg_handler_t *arg_handler,
                                  pfstest_value_t *actual),
    void (*matched_function)(pfstest_arg_handler_t *arg_handler,
                             pfstest_value_t *actual),
    void (*printer)(pfstest_arg_handler_t *arg_handler,
                    pfstest_reporter_t *reporter))
{
    pfstest_tagged_init((pfstest_tagged_t *)arg_handler,
                        &pfstest_arg_handler_tag);

    arg_handler->test_function = test_function;
    arg_handler->matched_function = matched_function;
    arg_handler->printer = printer;
}

pfstest_arg_handler_t *pfstest_arg_handler_new(
    pfstest_bool (*test_function)(pfstest_arg_handler_t *arg_handler,
                                  pfstest_value_t *actual),
    void (*matched_function)(pfstest_arg_handler_t *arg_handler,
                             pfstest_value_t *actual),
    void (*printer)(pfstest_arg_handler_t *arg_handler,
                    pfstest_reporter_t *reporter))
{
    pfstest_arg_handler_t *h = pfstest_alloc(sizeof(*h));

    pfstest_arg_handler_init(h, test_function, matched_function, printer);

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
