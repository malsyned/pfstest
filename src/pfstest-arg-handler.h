#ifndef PFSTEST_ARG_HANDLER_H
#define PFSTEST_ARG_HANDLER_H

#include "pfstest-platform.h"
#include "pfstest-value.h"
#include "pfstest-tag.h"

typedef struct _pfstest_arg_handler_t pfstest_arg_handler_t;

struct _pfstest_arg_handler_t
{
    pfstest_tagged_t tagged;
    pfstest_bool (*test_function)(pfstest_arg_handler_t *arg_handler,
                                  pfstest_value_t *actual);
    void (*matched_function)(pfstest_arg_handler_t *arg_handler,
                             pfstest_value_t *actual);
    void (*printer)(pfstest_arg_handler_t *arg_handler,
                    pfstest_reporter_t *reporter);
};

void pfstest_arg_handler_init(
    pfstest_arg_handler_t *arg_handler,
    pfstest_bool (*test_function)(pfstest_arg_handler_t *arg_handler,
                                  pfstest_value_t *actual),
    void (*matched_function)(pfstest_arg_handler_t *arg_handler,
                             pfstest_value_t *actual),
    void (*printer)(pfstest_arg_handler_t *arg_handler,
                    pfstest_reporter_t *reporter));
pfstest_arg_handler_t *pfstest_arg_handler_new(
    pfstest_bool (*test_function)(pfstest_arg_handler_t *arg_handler,
                                  pfstest_value_t *actual),
    void (*matched_function)(pfstest_arg_handler_t *arg_handler,
                             pfstest_value_t *actual),
    void (*printer)(pfstest_arg_handler_t *arg_handler,
                    pfstest_reporter_t *reporter));
void pfstest_arg_handler_print(pfstest_arg_handler_t *arg_handler,
                               pfstest_reporter_t *reporter);
pfstest_bool pfstest_arg_handler_test(pfstest_arg_handler_t *arg_handler,
                                      pfstest_value_t *actual);
void pfstest_arg_handler_matched(pfstest_arg_handler_t *arg_handler,
                                 pfstest_value_t *actual);
void *pfstest_arg_handler_data(pfstest_arg_handler_t *arg_handler);

extern pfstest_tag_t pfstest_arg_handler_tag;

#endif /* !PFSTEST_ARG_HANDLER_H */
