#include "pfstest-arg-handlers.h"

#include <string.h>
#include <stdio.h>

#include "pfstest-platform.h"
#include "pfstest-alloc.h"

/* arg_that */

static bool arg_that_test(pfstest_arg_handler_t *arg_handler,
                          pfstest_value_t *actual)
{
    pfstest_matcher_t *matcher = pfstest_arg_handler_data(arg_handler);
    return pfstest_matcher_matches(matcher, actual);
}

static void arg_that_matched(pfstest_arg_handler_t *arg_handler,
                             pfstest_value_t *actual)
{
    /* Do nothing */
}

static void arg_that_printer(pfstest_arg_handler_t *arg_handler)
{
    pfstest_matcher_t *matcher = pfstest_arg_handler_data(arg_handler);
    pfstest_matcher_print(matcher);
}

pfstest_arg_handler_t *pfstest_arg_that(pfstest_matcher_t *matcher)
{
    return arg_handler_new(arg_that_test,
                           arg_that_matched,
                           arg_that_printer,
                           (void *)matcher);
}

/* assign_arg */

static bool assign_arg_test(pfstest_arg_handler_t *arg_handler,
                            pfstest_value_t *actual)
{
    return true;
}

static void assign_arg_do_copy(pfstest_value_t *dest, pfstest_value_t *src)
{
    /* TODO: Validate the size of the destination, to catch test bugs */

    memcpy(pfstest_value_data(dest),
           pfstest_value_data(src),
           pfstest_value_size(src));
}

static void assign_arg_matched(pfstest_arg_handler_t *arg_handler,
                               pfstest_value_t *dest)
{
    pfstest_value_t *src = pfstest_arg_handler_data(arg_handler);

    assign_arg_do_copy(dest, src);
}

static void assign_arg_printer(pfstest_arg_handler_t *this)
{
    pfstest_printf_nv(pfstest_nv_string("is any pointer"));
}

pfstest_arg_handler_t *pfstest_assign_arg(pfstest_value_t *src)
{
    return arg_handler_new(assign_arg_test,
                           assign_arg_matched,
                           assign_arg_printer,
                           src);
}

struct assign_arg_that_args {
    pfstest_matcher_t *matcher;
    pfstest_value_t *src;
};

static bool assign_arg_that_test(pfstest_arg_handler_t *arg_handler,
                                 pfstest_value_t *actual)
{
    struct assign_arg_that_args *args =
        pfstest_arg_handler_data(arg_handler);
    return pfstest_matcher_matches(args->matcher, actual);
}

static void assign_arg_that_matched(pfstest_arg_handler_t *arg_handler,
                                    pfstest_value_t *dest)
{
    struct assign_arg_that_args *args =
        pfstest_arg_handler_data(arg_handler);
    assign_arg_do_copy(dest, args->src);
}

static void assign_arg_that_printer(pfstest_arg_handler_t *arg_handler)
{
    struct assign_arg_that_args *args =
        pfstest_arg_handler_data(arg_handler);
    pfstest_matcher_print(args->matcher);
}

pfstest_arg_handler_t *pfstest_assign_arg_that(pfstest_matcher_t *matcher,
                                               pfstest_value_t *src)
{
    struct assign_arg_that_args *args = pfstest_alloc(sizeof(*args));
    args->matcher = matcher;
    args->src = src;
    
    return arg_handler_new(assign_arg_that_test,
                           assign_arg_that_matched,
                           assign_arg_that_printer,
                           args);
}
