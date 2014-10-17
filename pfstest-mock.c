#include "pfstest-mock.h"

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

#include "pfstest-alloc.h"
#include "pfstest-core.h"

static pfstest_list_t expectations = PFSTEST_LIST_EMPTY();
static pfstest_list_t invocations = PFSTEST_LIST_EMPTY();
static pfstest_list_t verifiers = PFSTEST_LIST_EMPTY();

void pfstest_mock_init(void)
{
    pfstest_list_reset(&expectations);
    pfstest_list_reset(&invocations);
    pfstest_list_reset(&verifiers);
}

static void expectation_add(pfstest_expectation_t *expectation)
{
    pfstest_list_append(&expectations, (pfstest_list_node_t *)expectation);
}

static void invocation_add(pfstest_invocation_t *invocation)
{
    pfstest_list_append(&invocations, (pfstest_list_node_t *)invocation);
}

static void verifier_add(pfstest_verifier_t *verifier)
{
    pfstest_list_append(&verifiers, (pfstest_list_node_t *)verifier);
}

/* mock */

static int pfstest_mock_arg_count(const pfstest_mock_t *mock)
{
    return mock->arg_count;
}

/* expectation */

#define PFSTEST_EXPECTATION_DO_TIMES_INFINITE -1

static pfstest_expectation_t *pfstest_expectation_new(
    const pfstest_mock_t *mock, pfstest_arg_handler_t **arg_handlers)
{
    pfstest_expectation_t *e = pfstest_alloc(sizeof(*e));
    pfstest_list_node_init((pfstest_list_node_t *)e);
    e->mock = mock;
    e->arg_handlers = arg_handlers;
    e->return_value = NULL;
    e->times = PFSTEST_EXPECTATION_DO_TIMES_INFINITE;

    return e;
}

static void pfstest_expectation_print(pfstest_expectation_t *e)
{
    int i;

    printf("%s with (", e->mock->name);
    for (i = 0; i < e->mock->arg_count; i++) {
        printf("%s ", e->mock->arg_names[i]);
        pfstest_arg_handler_print(e->arg_handlers[i]);

        if (i < e->mock->arg_count - 1)
            printf(", ");
    }
    printf(")");
}

pfstest_expectation_t *pfstest_do_return(pfstest_value_t *return_value,
                                         pfstest_expectation_t *expectation)
{
    expectation->return_value = return_value;

    return expectation;
}

pfstest_expectation_t *pfstest_one_time(pfstest_expectation_t *expectation)
{
    expectation->times = 1;

    return expectation;
}

pfstest_expectation_t *pfstest_do_times(int times,
                                        pfstest_expectation_t *expectation)
{
    expectation->times = times;

    return expectation;
}

pfstest_expectation_t *pfstest_when(const pfstest_mock_t *mock, ...)
{
    pfstest_arg_handler_t **arg_handlers;
    pfstest_expectation_t *expectation;
    int arg_count;
    va_list ap;
    int i;

    arg_count = pfstest_mock_arg_count(mock);
    arg_handlers = pfstest_alloc(sizeof(*arg_handlers) * arg_count);

    va_start(ap, mock);
    for (i = 0; i < arg_count; i++) {
        arg_handlers[i] = va_arg(ap, pfstest_arg_handler_t *);
    }
    va_end(ap);

    expectation = pfstest_expectation_new(mock, arg_handlers);
    expectation_add(expectation);

    return expectation;
}

/* invocation */

static pfstest_invocation_t *invocation_new(pfstest_expectation_t *e)
{
    pfstest_invocation_t *i = pfstest_alloc(sizeof(*i));
    pfstest_list_node_init((pfstest_list_node_t *)i);
    i->expectation = e;

    return i;
}

static bool args_match(int arg_count,
                       pfstest_value_t **args,
                       pfstest_arg_handler_t **arg_handlers)
{
    int i;

    for (i = 0; i < arg_count; i++) {
        if (!pfstest_arg_handler_test(arg_handlers[i], args[i]))
            return false;
    }

    return true;
}

static void args_matched(int arg_count,
                         pfstest_value_t **args,
                         pfstest_arg_handler_t **arg_handlers)
{
    int i;

    for (i = 0; i < arg_count; i++) {
        pfstest_arg_handler_matched(arg_handlers[i], args[i]);
    }
}

pfstest_value_t *pfstest_mock_invoke(const pfstest_mock_t *mock,
                                     pfstest_value_t *default_return_value,
                                     ...)
{
    pfstest_list_node_t *expectation_node = pfstest_list_head(&expectations);
    int arg_count = pfstest_mock_arg_count(mock);
    pfstest_value_t **args = pfstest_alloc(sizeof(*args) * arg_count);
    pfstest_value_t *return_value = default_return_value;
    va_list ap;
    int i;

    va_start(ap, default_return_value);
    for(i = 0; i < arg_count; i++) {
        args[i] = va_arg(ap, pfstest_value_t *);
    }
    va_end(ap);

    while (expectation_node != NULL) {
        pfstest_expectation_t *e = (pfstest_expectation_t *)expectation_node;

        if (e->mock == mock
            && e->times != 0
            && args_match(arg_count, args, e->arg_handlers))
        {
            args_matched(arg_count, args, e->arg_handlers);

            invocation_add(invocation_new(e));

            if (e->return_value != NULL)
                return_value = e->return_value;

            if (e->times != PFSTEST_EXPECTATION_DO_TIMES_INFINITE)
                e->times--;

            break;
        }
        expectation_node = expectation_node->next;
    }
    
    return return_value;
}

/* verify */

static pfstest_verifier_t *verifier_new(
    void (*function)(pfstest_verifier_t *verifier),
    void *data)
{
    pfstest_verifier_t *v = pfstest_alloc(sizeof(*v));
    pfstest_list_node_init((pfstest_list_node_t *)v);
    v->function = function;
    v->data = data;

    return v;
}

void pfstest_run_verifiers(void)
{
    pfstest_list_node_t *verifier_node = pfstest_list_head(&verifiers);

    while (verifier_node != NULL) {
        pfstest_verifier_t *v = (pfstest_verifier_t *)verifier_node;

        v->function(v);

        verifier_node = verifier_node->next;
    }
}

/* basic verifier */

struct do_verification_printer_args 
{
    pfstest_expectation_t *expectation;
    int invocation_count;
    int wanted_count;
};

static void wrong_call_count_printer(const void *data)
{
    const struct do_verification_printer_args *args = data;

    if (args->invocation_count == 0) {
        printf("    Never called ");
        pfstest_expectation_print(args->expectation);
    } else if (args->invocation_count != 1) {
        printf("    Wanted ");
        pfstest_expectation_print(args->expectation);
        printf(" %d time%s\n",
               args->wanted_count,
               args->wanted_count == 1 ? "" : "s");
        printf("    Was called %d time%s",
               args->invocation_count,
               args->invocation_count == 1 ? "" : "s");
    }
}

void pfstest_verify_at_location(const char *file, int line,
                                pfstest_expectation_t *e)
{
    pfstest_verify_times_at_location(file, line, exactly(1), e);
}

struct verify_with_mode_args
{
    const char *file;
    int line;
    pfstest_verify_mode_t *mode;
    pfstest_expectation_t *expectation;
};

static void do_verification_with_mode(pfstest_verifier_t *v)
{
    struct verify_with_mode_args *args = v->data;

    args->mode->function(args->file, args->line,
                         args->mode, args->expectation);
}

void pfstest_verify_times_at_location(const char *file, int line,
                                      pfstest_verify_mode_t *mode,
                                      pfstest_expectation_t *e)
{
    struct verify_with_mode_args *args = pfstest_alloc(sizeof(*args));
    args->file = file;
    args->line = line;
    args->mode = mode;
    args->expectation = e;
    
    verifier_add(verifier_new(do_verification_with_mode, args));
}

static void do_exactly(const char *file, int line,
                       pfstest_verify_mode_t *mode,
                       pfstest_expectation_t *expectation)
{
    int wanted_count = *(int *)mode->data;
    int invocation_count = 0;

    pfstest_list_node_t *invocation_node = pfstest_list_head(&invocations);
    while (invocation_node != NULL) {
        pfstest_invocation_t *i = (pfstest_invocation_t *)invocation_node;

        if (expectation == i->expectation) {
            invocation_count++;
        }

        invocation_node = invocation_node->next;
    }

    if (invocation_count != wanted_count) {
        struct do_verification_printer_args printer_args = {
            expectation, invocation_count, wanted_count,
        };
        
        pfstest_fail_with_printer(file, line,
                                  wrong_call_count_printer, &printer_args);
    }
}

pfstest_verify_mode_t *pfstest_exactly(int times)
{
    pfstest_verify_mode_t *mode = pfstest_alloc(sizeof(*mode));
    int *data = pfstest_alloc(sizeof(*data));
    *data = times;

    mode->function = do_exactly;
    mode->data = data;

    return mode;
}

/* in_order verifier */

struct in_order_fail_printer_args
{
    pfstest_expectation_t *expectation;
    pfstest_expectation_t *prev_expectation;
};

static void in_order_fail_printer(const void *data)
{
    const struct in_order_fail_printer_args *args = data;

    printf("    Not called in order: ");
    pfstest_expectation_print(args->expectation);
    if (args->prev_expectation != NULL) {
        printf("\n");
        printf("    Expected after: ");
        pfstest_expectation_print(args->prev_expectation);
    }
}

struct in_order_expectation
{
    pfstest_list_node_t node;
    const char *file;
    int line;
    pfstest_expectation_t *expectation;
};

static void do_in_order_verification(pfstest_verifier_t *v)
{
    in_order_t *order = v->data;
    pfstest_list_node_t *invocation_node = pfstest_list_head(&invocations);
    pfstest_list_node_t *in_order_expectation_node =
        pfstest_list_head(&order->expectations);
    pfstest_expectation_t *prev_expectation = NULL;
    struct in_order_expectation *in_order_expectation;

    while (invocation_node != NULL && in_order_expectation_node != NULL) {
        in_order_expectation =
            (struct in_order_expectation *)in_order_expectation_node;
        pfstest_invocation_t *invocation =
            (pfstest_invocation_t *)invocation_node;

        if (in_order_expectation->expectation == invocation->expectation) {
            prev_expectation = in_order_expectation->expectation;
            in_order_expectation_node = in_order_expectation_node->next;
        }
        invocation_node = invocation_node->next;
    }

    if (in_order_expectation_node != NULL) {
        in_order_expectation =
            (struct in_order_expectation *)in_order_expectation_node;
        struct in_order_fail_printer_args printer_args = {
            in_order_expectation->expectation, prev_expectation,
        };
        
        pfstest_fail_with_printer(in_order_expectation->file,
                                  in_order_expectation->line,
                                  in_order_fail_printer,
                                  &printer_args);
    }
}

pfstest_in_order_t *pfstest_in_order_new(void)
{
    pfstest_in_order_t *order = pfstest_alloc(sizeof(*order));

    pfstest_list_reset(&order->expectations);
    verifier_add(verifier_new(do_in_order_verification, order));

    return order;
}

void pfstest_in_order_verify_at_location(const char *file, int line,
                                         pfstest_in_order_t *order,
                                         pfstest_expectation_t *expectation)
{
    struct in_order_expectation *ioe = pfstest_alloc(sizeof(*ioe));
    pfstest_list_node_init((pfstest_list_node_t *)ioe);
    ioe->file = file;
    ioe->line = line;
    ioe->expectation = expectation;

    pfstest_list_append(&order->expectations, (pfstest_list_node_t *)ioe);
}
