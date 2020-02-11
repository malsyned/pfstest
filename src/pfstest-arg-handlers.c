#include "pfstest-arg-handlers.h"

#include <string.h>
#include <stdio.h>

#include "pfstest-platform.h"
#include "pfstest-alloc.h"

/* arg_that */

struct arg_that_handler
{
    pfstest_arg_handler_t parent;
    pfstest_matcher_t *matcher;
};

static pfstest_bool arg_that_test(pfstest_arg_handler_t *arg_handler,
                                  pfstest_value_t *actual)
{
    struct arg_that_handler *arg_that_handler =
        (struct arg_that_handler *)arg_handler;
    return pfstest_matcher_matches(arg_that_handler->matcher, actual);
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
    struct arg_that_handler *arg_that_handler =
        (struct arg_that_handler *)arg_handler;
    pfstest_matcher_print(arg_that_handler->matcher, reporter);
}

pfstest_arg_handler_t *pfstest_arg_that(pfstest_matcher_t *matcher)
{
    struct arg_that_handler *h = pfstest_alloc(sizeof(*h));
    pfstest_arg_handler_init((pfstest_arg_handler_t *)h,
                             arg_that_test,
                             arg_that_matched,
                             arg_that_printer);
    h->matcher = matcher;

    return (pfstest_arg_handler_t *)h;
}

/* assign_arg */

struct assign_arg_handler
{
    pfstest_arg_handler_t parent;
    pfstest_value_t *src;
};

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
    struct assign_arg_handler *assign_arg_handler =
        (struct assign_arg_handler *)arg_handler;
    
    assign_arg_do_copy(dest, assign_arg_handler->src);
}

static void assign_arg_printer(pfstest_arg_handler_t *this,
                               pfstest_reporter_t *reporter)
{
    (void)this;
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("any pointer"));
}

pfstest_arg_handler_t *pfstest_assign_arg(pfstest_value_t *src)
{
    struct assign_arg_handler *h = pfstest_alloc(sizeof(*h));
    pfstest_arg_handler_init((pfstest_arg_handler_t *)h,
                             assign_arg_test,
                             assign_arg_matched,
                             assign_arg_printer);
    h->src = src;
    return (pfstest_arg_handler_t *)h;
}

/* assign_arg_that */

struct assign_arg_that_handler
{
    pfstest_arg_handler_t parent;
    pfstest_matcher_t *matcher;
    pfstest_value_t *src;
};

static pfstest_bool assign_arg_that_test(pfstest_arg_handler_t *arg_handler,
                                         pfstest_value_t *actual)
{
    struct assign_arg_that_handler *assign_arg_that_handler=
        (struct assign_arg_that_handler *)arg_handler;
    return pfstest_matcher_matches(assign_arg_that_handler->matcher, actual);
}

static void assign_arg_that_matched(pfstest_arg_handler_t *arg_handler,
                                    pfstest_value_t *dest)
{
    struct assign_arg_that_handler *assign_arg_that_handler=
        (struct assign_arg_that_handler *)arg_handler;
    assign_arg_do_copy(dest, assign_arg_that_handler->src);
}

static void assign_arg_that_printer(pfstest_arg_handler_t *arg_handler,
                                    pfstest_reporter_t *reporter)
{
    struct assign_arg_that_handler *assign_arg_that_handler=
        (struct assign_arg_that_handler *)arg_handler;
    pfstest_matcher_print(assign_arg_that_handler->matcher, reporter);
}

pfstest_arg_handler_t *pfstest_assign_arg_that(pfstest_matcher_t *matcher,
                                               pfstest_value_t *src)
{
    struct assign_arg_that_handler *h = pfstest_alloc(sizeof(*h));
    pfstest_arg_handler_init((pfstest_arg_handler_t *)h,
                             assign_arg_that_test,
                             assign_arg_that_matched,
                             assign_arg_that_printer);
    h->matcher = matcher;
    h->src = src;

    return (pfstest_arg_handler_t *)h;
}

/* capture_arg */

struct capture_arg_handler
{
    pfstest_arg_handler_t parent;
    void *arg_p;
};

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
    struct capture_arg_handler *capture_arg_handler =
        (struct capture_arg_handler *)arg_handler;
    void *arg_p = capture_arg_handler->arg_p;

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
    struct capture_arg_handler *h = pfstest_alloc(sizeof(*h));
    pfstest_arg_handler_init((pfstest_arg_handler_t *)h,
                             capture_arg_test,
                             capture_arg_matched,
                             capture_arg_printer);
    h->arg_p = arg_p;

    return (pfstest_arg_handler_t *)h;
}
