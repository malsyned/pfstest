#include "pfstest-arg-handlers.h"

#include <string.h>
#include <stdio.h>

#include "pfstest-platform.h"
#include "pfstest-alloc.h"

/* arg_that */

static pfstest_bool arg_that_test(pfstest_arg_handler_t *arg_handler,
                                  pfstest_value_t *actual)
{
    pfstest_matcher_t *matcher = pfstest_arg_handler_data(arg_handler);
    return pfstest_matcher_matches(matcher, actual);
}

static void arg_that_matched(pfstest_arg_handler_t *arg_handler,
                             pfstest_value_t *actual)
{
    /* Do nothing */
    (void)arg_handler;
    (void)actual;
}

static void arg_that_printer(pfstest_arg_handler_t *arg_handler,
                             pfstest_reporter_t *reporter)
{
    pfstest_matcher_t *matcher = pfstest_arg_handler_data(arg_handler);
    pfstest_matcher_print(matcher, reporter);
}

pfstest_arg_handler_t *pfstest_arg_that(pfstest_matcher_t *matcher)
{
    return pfstest_arg_handler_new(arg_that_test,
                                   arg_that_matched,
                                   arg_that_printer,
                                   (void *)matcher);
}

/* assign_arg */

static pfstest_bool assign_arg_test(pfstest_arg_handler_t *arg_handler,
                                    pfstest_value_t *actual)
{
    (void)arg_handler;
    (void)actual;
    return pfstest_true;
}

static void assign_arg_do_copy(pfstest_value_t *dest, pfstest_value_t *src)
{
    /* TODO: Validate the size of the destination, to catch test bugs */

    /* This (void *) cast removes the const qualifier and then memcpy
     * writes to the underlying data object. This is safe provided
     * that the original data that was boxed into the pfstest_value
     * was not const data. It is up to the user to make sure that they
     * don't use assign-through-pointer argument handlers to write to
     * const memory and thereby run afoul of undefined behavior. */
    memcpy((void *)pfstest_value_data(dest),
           pfstest_value_data(src),
           pfstest_value_size(src));
}

static void assign_arg_matched(pfstest_arg_handler_t *arg_handler,
                               pfstest_value_t *dest)
{
    pfstest_value_t *src = pfstest_arg_handler_data(arg_handler);

    assign_arg_do_copy(dest, src);
}

static void assign_arg_printer(pfstest_arg_handler_t *this,
                               pfstest_reporter_t *reporter)
{
    (void)this;
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("any pointer"));
}

pfstest_arg_handler_t *pfstest_assign_arg(pfstest_value_t *src)
{
    return pfstest_arg_handler_new(assign_arg_test,
                                   assign_arg_matched,
                                   assign_arg_printer,
                                   src);
}

/* assign_arg_that */

struct assign_arg_that_args {
    pfstest_matcher_t *matcher;
    pfstest_value_t *src;
};

static pfstest_bool assign_arg_that_test(pfstest_arg_handler_t *arg_handler,
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

static void assign_arg_that_printer(pfstest_arg_handler_t *arg_handler,
                                    pfstest_reporter_t *reporter)
{
    struct assign_arg_that_args *args =
        pfstest_arg_handler_data(arg_handler);
    pfstest_matcher_print(args->matcher, reporter);
}

pfstest_arg_handler_t *pfstest_assign_arg_that(pfstest_matcher_t *matcher,
                                               pfstest_value_t *src)
{
    struct assign_arg_that_args *args = pfstest_alloc(sizeof(*args));
    args->matcher = matcher;
    args->src = src;
    
    return pfstest_arg_handler_new(assign_arg_that_test,
                                   assign_arg_that_matched,
                                   assign_arg_that_printer,
                                   args);
}

/* capture_arg */

static pfstest_bool capture_arg_test(pfstest_arg_handler_t *arg_handler,
                                     pfstest_value_t *actual)
{
    (void)arg_handler;
    (void)actual;
    return pfstest_true;
}

static pfstest_bool value_is_pointer(pfstest_value_t *actual)
{
    return (pfstest_value_size(actual) == 0);
}

static void copy_pointer(void *arg_p, pfstest_value_t *pointer)
{
    const void *data = pfstest_value_data(pointer);
    memcpy(arg_p, &data, sizeof(data));
}

static void copy_blob(void *arg_p, pfstest_value_t *blob)
{
    memcpy(arg_p, pfstest_value_data(blob), pfstest_value_size(blob));
}

static void capture_arg_matched(pfstest_arg_handler_t *arg_handler,
                                pfstest_value_t *actual)
{
    void *arg_p = pfstest_arg_handler_data(arg_handler);

    if (value_is_pointer(actual))
        copy_pointer(arg_p, actual);
    else
        copy_blob(arg_p, actual);
}

static void capture_arg_printer(pfstest_arg_handler_t *arg_handler,
                                    pfstest_reporter_t *reporter)
{
    (void)arg_handler;
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("anything"));
}

pfstest_arg_handler_t *pfstest_capture_arg(void *arg_p)
{
    return pfstest_arg_handler_new(capture_arg_test,
                                   capture_arg_matched,
                                   capture_arg_printer,
                                   arg_p);
}
