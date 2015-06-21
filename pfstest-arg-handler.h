#ifndef PFSTEST_ARG_HANDLER_H
#define PFSTEST_ARG_HANDLER_H

#include "pfstest-platform.h"
#include "pfstest-value.h"

typedef struct _pfstest_arg_handler_t pfstest_arg_handler_t;

struct _pfstest_arg_handler_t
{
    bool (*test_function)(pfstest_arg_handler_t *arg_handler,
                          pfstest_value_t *actual);
    void (*matched_function)(pfstest_arg_handler_t *arg_handler,
                             pfstest_value_t *actual);
    void (*printer)(pfstest_output_formatter_t *formatter,
                    pfstest_arg_handler_t *arg_handler);
    void *data;
};

pfstest_arg_handler_t *pfstest_arg_handler_new(
    bool (*test_function)(pfstest_arg_handler_t *arg_handler,
                          pfstest_value_t *actual),
    void (*matched_function)(pfstest_arg_handler_t *arg_handler,
                             pfstest_value_t *actual),
    void (*printer)(pfstest_output_formatter_t *formatter,
                    pfstest_arg_handler_t *arg_handler),
    void *data);
void pfstest_arg_handler_print(pfstest_output_formatter_t *formatter,
                               pfstest_arg_handler_t *arg_handler);
bool pfstest_arg_handler_test(pfstest_arg_handler_t *arg_handler,
                              pfstest_value_t *actual);
void pfstest_arg_handler_matched(pfstest_arg_handler_t *arg_handler,
                                 pfstest_value_t *actual);
void *pfstest_arg_handler_data(pfstest_arg_handler_t *arg_handler);

#endif /* !PFSTEST_ARG_HANDLER_H */
