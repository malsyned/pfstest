#include "pfstest-mock.h"

#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>

#include "pfstest-alloc.h"
#include "pfstest-core.h"

typedef struct _dynamic_env_t
{
    struct _dynamic_env_t *next;
    pfstest_list_t expectations;
    pfstest_list_t invocations;
    pfstest_list_t verifiers;
    pfstest_list_t default_expectations;
} dynamic_env_t;

static dynamic_env_t *dynamic_env = NULL;

pfstest_plugin_define(pfstest_mock_plugin,
                      pfstest_mock_setup,
                      pfstest_mock_run_verifiers,
                      pfstest_mock_teardown);
pfstest_plugin_autoload(pfstest_mock_plugin);

void pfstest_mock_setup(void)
{
    dynamic_env_t *new_frame = pfstest_alloc(sizeof(*new_frame));
    new_frame->next = dynamic_env;
    dynamic_env = new_frame;
    
    pfstest_list_reset(&dynamic_env->expectations);
    pfstest_list_reset(&dynamic_env->invocations);
    pfstest_list_reset(&dynamic_env->verifiers);
    pfstest_list_reset(&dynamic_env->default_expectations);
}

void pfstest_mock_teardown(void)
{
    dynamic_env = dynamic_env->next;
}

static void expectation_add(pfstest_expectation_t *expectation)
{
    pfstest_list_append(&dynamic_env->expectations, (pfstest_list_node_t *)expectation);
}

static void invocation_add(pfstest_invocation_t *invocation)
{
    pfstest_list_append(&dynamic_env->invocations,
                        (pfstest_list_node_t *)invocation);
}

static void verifier_add(pfstest_verifier_t *verifier)
{
    pfstest_list_append(&dynamic_env->verifiers,
                        (pfstest_list_node_t *)verifier);
}

static void default_expectation_add(pfstest_expectation_t *expectation)
{
    pfstest_list_append(&dynamic_env->default_expectations,
                        (pfstest_list_node_t *)expectation);
}

/* mock */

static unsigned int  pfstest_mock_arg_count(
    const pfstest_pg_ptr pfstest_mock_t *mock)
{
    pfstest_mock_t m;
    pfstest_memcpy_pg(&m, mock, sizeof(m));

    return m.arg_count;
}

static const pfstest_pg_ptr char *pfstest_mock_name(
    const pfstest_pg_ptr pfstest_mock_t *mock)
{
    pfstest_mock_t m;
    pfstest_memcpy_pg(&m, mock, sizeof(m));

    return m.name;
}

/* expectation */

#define PFSTEST_EXPECTATION_DO_TIMES_INFINITE -1

static pfstest_expectation_t *pfstest_expectation_new(
    const pfstest_pg_ptr pfstest_mock_t *mock,
    pfstest_arg_handler_t **arg_handlers)
{
    pfstest_expectation_t *e = pfstest_alloc(sizeof(*e));
    pfstest_list_node_init((pfstest_list_node_t *)e);
    e->mock = mock;
    e->arg_handlers = arg_handlers;
    e->return_value = NULL;
    e->times = PFSTEST_EXPECTATION_DO_TIMES_INFINITE;

    return e;
}

static void pfstest_expectation_print(pfstest_expectation_t *e,
                                      pfstest_reporter_t *reporter)
{
    unsigned int i;

    pfstest_reporter_print_pg_str(reporter, pfstest_mock_name(e->mock));
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(" with ("));

    for (i = 0; i < pfstest_mock_arg_count(e->mock); i++) {
        pfstest_arg_handler_print(e->arg_handlers[i], reporter);

        if (i < pfstest_mock_arg_count(e->mock) - 1)
            pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(", "));
    }
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(")"));
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

pfstest_expectation_t *pfstest_when(
    const pfstest_pg_ptr pfstest_mock_t *mock, ...)
{
    pfstest_arg_handler_t **arg_handlers;
    pfstest_expectation_t *expectation;
    unsigned int arg_count;
    va_list ap;
    unsigned int i;

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
    i->mark = pfstest_false;

    return i;
}

static pfstest_bool args_match(unsigned int arg_count,
                               pfstest_value_t **args,
                               pfstest_arg_handler_t **arg_handlers)
{
    unsigned int i;

    /* The default expectation, which should match everything, has
     * NULL arg_handlers */
    if (arg_handlers == NULL) {
        return pfstest_true;
    }

    for (i = 0; i < arg_count; i++) {
        if (!pfstest_arg_handler_test(arg_handlers[i], args[i]))
            return pfstest_false;
    }

    return pfstest_true;
}

static void args_matched(unsigned int arg_count,
                         pfstest_value_t **args,
                         pfstest_arg_handler_t **arg_handlers)
{
    unsigned int i;

    for (i = 0; i < arg_count; i++) {
        pfstest_arg_handler_matched(arg_handlers[i], args[i]);
    }
}

static pfstest_expectation_t *find_default_expectation(
    const pfstest_pg_ptr pfstest_mock_t *mock)
{
    pfstest_expectation_t *expectation;

    pfstest_list_iter (expectation, &dynamic_env->default_expectations) {
        if (mock == expectation->mock)
            return expectation;
    }

    return NULL;
}

static pfstest_expectation_t *get_default_expectation(
    const pfstest_pg_ptr pfstest_mock_t *mock)
{
    pfstest_expectation_t *e = find_default_expectation(mock);

    if (e == NULL) {
        /* If arg_handlers is NULL, args_match will always match */
        e = pfstest_expectation_new(mock, NULL);
        default_expectation_add(e);
    }

    return e;
}

pfstest_value_t *pfstest_mock_invoke(
    const pfstest_pg_ptr pfstest_mock_t *mock,
    pfstest_value_t *default_return_value,
    ...)
{
    pfstest_expectation_t *e;
    unsigned int arg_count = pfstest_mock_arg_count(mock);
    pfstest_value_t **args = pfstest_alloc(sizeof(*args) * arg_count);
    pfstest_value_t *return_value = default_return_value;
    va_list ap;
    unsigned int i;

    va_start(ap, default_return_value);
    for(i = 0; i < arg_count; i++) {
        args[i] = va_arg(ap, pfstest_value_t *);
    }
    va_end(ap);

    pfstest_list_iter (e, &dynamic_env->expectations) {
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
    }
    if (e == NULL) {
        invocation_add(invocation_new(get_default_expectation(mock)));
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

void pfstest_mock_run_verifiers(void)
{
    pfstest_verifier_t *verifier;

    pfstest_list_iter (verifier, &dynamic_env->verifiers) {
        verifier->function(verifier);
    }
}

/* basic verifier */

struct do_verification_printer_args 
{
    pfstest_expectation_t *expectation;
    int invocation_count;
    const pfstest_pg_ptr char *wanted_desc_prefix;
    int wanted_count;
};

static void print_plural(pfstest_reporter_t *reporter, int count)
{
    if (count != 1) {
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("s"));
    }
}

static void wrong_call_count_printer(pfstest_reporter_t *reporter,
                                     const void *data)
{
    const struct do_verification_printer_args *args = data;

    if (args->invocation_count == 0) {
        pfstest_reporter_print_pg_str(reporter,
                                      pfstest_pg_str("Never called "));
        pfstest_expectation_print(args->expectation, reporter);
    } else if (args->invocation_count != 1) {
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("Wanted "));
        pfstest_expectation_print(args->expectation, reporter);

        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(" "));
        pfstest_reporter_print_pg_str(reporter, args->wanted_desc_prefix);
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(" "));
        pfstest_reporter_print_int(reporter, args->wanted_count);
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(" time"));
        print_plural(reporter, args->wanted_count);
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("\n"));

        pfstest_reporter_print_pg_str(reporter,
                                      pfstest_pg_str("Was called "));
        pfstest_reporter_print_int(reporter, args->invocation_count);
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(" time"));
        print_plural(reporter, args->invocation_count);
    }
}

void pfstest_verify_at_location(const pfstest_pg_ptr char *file, int line,
                                pfstest_expectation_t *e)
{
    pfstest_verify_times_at_location(file, line, exactly(1), e);
}

struct verify_with_mode_args
{
    const pfstest_pg_ptr char *file;
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

void pfstest_verify_times_at_location(const pfstest_pg_ptr char *file,
                                      int line,
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

static int count_and_mark_invocations(pfstest_expectation_t *expectation)
{
    int invocation_count = 0;
    pfstest_invocation_t *invocation;

    pfstest_list_iter (invocation, &dynamic_env->invocations) {
        if (expectation == invocation->expectation) {
            invocation->mark = pfstest_true;
            invocation_count++;
        }
    }

    return invocation_count;
}

static void fail_wrong_call_count(
    const pfstest_pg_ptr char *file, int line,
    pfstest_expectation_t *expectation,
    int invocation_count,
    const pfstest_pg_ptr char *wanted_desc_prefix,
    int wanted_count)
{
    struct do_verification_printer_args printer_args;
    printer_args.expectation = expectation;
    printer_args.invocation_count = invocation_count;
    printer_args.wanted_desc_prefix = wanted_desc_prefix;
    printer_args.wanted_count = wanted_count;
        
    pfstest_fail_with_printer(file, line,
                              wrong_call_count_printer,
                              (const void *)&printer_args);
}

static void do_exactly(const pfstest_pg_ptr char *file, int line,
                       pfstest_verify_mode_t *mode,
                       pfstest_expectation_t *expectation)
{
    int wanted_count = *(int *)mode->data;
    int invocation_count = count_and_mark_invocations(expectation);

    if (invocation_count != wanted_count) {
        fail_wrong_call_count(file, line, expectation, invocation_count,
                              pfstest_pg_str("exactly"), wanted_count);
    }
}

static pfstest_verify_mode_t *counting_mode_new(
    int times,
    void function(const pfstest_pg_ptr char *file, int line,
                  pfstest_verify_mode_t *mode,
                  pfstest_expectation_t *expectation))
{
    pfstest_verify_mode_t *mode = pfstest_alloc(sizeof(*mode));
    int *data = pfstest_alloc(sizeof(*data));
    *data = times;

    mode->function = function;
    mode->data = data;

    return mode;
}

pfstest_verify_mode_t *pfstest_exactly(int times)
{
    return counting_mode_new(times, do_exactly);
}

static void do_at_most(const pfstest_pg_ptr char *file, int line,
                       pfstest_verify_mode_t *mode,
                       pfstest_expectation_t *expectation)
{
    int wanted_count = *(int *)mode->data;
    int invocation_count = count_and_mark_invocations(expectation);

    if (invocation_count > wanted_count) {
        fail_wrong_call_count(file, line, expectation, invocation_count,
                              pfstest_pg_str("at most"), wanted_count);
    }
}

pfstest_verify_mode_t *pfstest_at_most(int times)
{
    return counting_mode_new(times, do_at_most);
}

static void do_at_least(const pfstest_pg_ptr char *file, int line,
                        pfstest_verify_mode_t *mode,
                        pfstest_expectation_t *expectation)
{
    int wanted_count = *(int *)mode->data;
    int invocation_count = count_and_mark_invocations(expectation);

    if (invocation_count < wanted_count) {
        fail_wrong_call_count(file, line, expectation, invocation_count,
                              pfstest_pg_str("at least"), wanted_count);
    }
}

pfstest_verify_mode_t *pfstest_at_least(int times)
{
    return counting_mode_new(times, do_at_least);
}

struct no_more_interactions_printer_args
{
    const pfstest_pg_ptr pfstest_mock_t *mock;
};

static void no_more_interactions_printer(
    pfstest_reporter_t *reporter, const void *data)
{
    const struct no_more_interactions_printer_args *args = data;

    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("Unexpected interactions with "));
    pfstest_reporter_print_pg_str(reporter, pfstest_mock_name(args->mock));
}

struct no_more_interactions_args
{
    const pfstest_pg_ptr pfstest_mock_t *mock;
    const pfstest_pg_ptr char *file;
    int line;
};

static void do_verify_no_more_interactions(pfstest_verifier_t *v)
{
    struct no_more_interactions_args *args = v->data;
    pfstest_invocation_t *i;

    pfstest_list_iter (i, &dynamic_env->invocations) {
        struct no_more_interactions_printer_args printer_args;
        printer_args.mock = args->mock;

        if (args->mock == i->expectation->mock && !i->mark) {
            pfstest_fail_with_printer(args->file, args->line,
                                      no_more_interactions_printer,
                                      (const void *)&printer_args);
        }
    }
}

void pfstest_verify_no_more_interactions_at_location(
    const pfstest_pg_ptr char *file,
    int line,
    const pfstest_pg_ptr pfstest_mock_t *mock)
{
    struct no_more_interactions_args *args = pfstest_alloc(sizeof(*args));
    args->mock = mock;
    args->file = file;
    args->line = line;

    verifier_add(verifier_new(do_verify_no_more_interactions, args));
}

/* No more invocations verification */

struct no_more_invocations_args
{
    const pfstest_pg_ptr char *file;
    int line;
};

static void do_verify_no_more_invocations(pfstest_verifier_t *v)
{
    struct no_more_invocations_args *args = v->data;
    pfstest_invocation_t *invocation;

    pfstest_list_iter (invocation, &dynamic_env->invocations) {
        if (!invocation->mark) {
            pfstest_fail_at_location(args->file, args->line,
                                     "Unexpected mock invocations");
        }
    }
}

void pfstest_verify_no_more_invocations_at_location(
    const pfstest_pg_ptr char *file, int line)
{
    struct no_more_invocations_args *args = pfstest_alloc(sizeof(*args));
    args->file = file;
    args->line = line;

    verifier_add(verifier_new(do_verify_no_more_invocations, args));
}

/* in_order verifier */

struct in_order_fail_printer_args
{
    pfstest_expectation_t *expectation;
    pfstest_expectation_t *prev_expectation;
};

static void in_order_fail_printer(pfstest_reporter_t *reporter,
                                  const void *data)
{
    const struct in_order_fail_printer_args *args = data;

    pfstest_reporter_print_pg_str(reporter,
                                  pfstest_pg_str("Not called in order: "));
    pfstest_expectation_print(args->expectation, reporter);
    if (args->prev_expectation != NULL) {
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("\n"));
        pfstest_reporter_print_pg_str(reporter,
                                      pfstest_pg_str("Expected after: "));
        pfstest_expectation_print(args->prev_expectation, reporter);
    }
}

struct in_order_expectation
{
    pfstest_list_node_t node;
    const pfstest_pg_ptr char *file;
    int line;
    pfstest_expectation_t *expectation;
};

static void do_in_order_verification(pfstest_verifier_t *v)
{
    in_order_t *order = v->data;
    pfstest_invocation_t *invocation;
    pfstest_list_node_t *in_order_expectation_node =
        pfstest_list_head(&order->expectations);
    pfstest_expectation_t *prev_expectation = NULL;
    struct in_order_expectation *in_order_expectation;

    pfstest_list_iter (invocation, &dynamic_env->invocations) {
        if (in_order_expectation_node == NULL)
            break;

        in_order_expectation =
            (struct in_order_expectation *)in_order_expectation_node;

        if (in_order_expectation->expectation == invocation->expectation) {
            invocation->mark = pfstest_true;
            prev_expectation = in_order_expectation->expectation;
            in_order_expectation_node = in_order_expectation_node->next;
        }
    }

    if (in_order_expectation_node != NULL) {
        struct in_order_fail_printer_args printer_args;

        in_order_expectation =
            (struct in_order_expectation *)in_order_expectation_node;

        printer_args.expectation = in_order_expectation->expectation;
        printer_args.prev_expectation = prev_expectation;
        
        pfstest_fail_with_printer(in_order_expectation->file,
                                  in_order_expectation->line,
                                  in_order_fail_printer,
                                  (const void *)&printer_args);
    }
}

pfstest_in_order_t *pfstest_in_order_new(void)
{
    pfstest_in_order_t *order = pfstest_alloc(sizeof(*order));

    pfstest_list_reset(&order->expectations);
    verifier_add(verifier_new(do_in_order_verification, order));

    return order;
}

void pfstest_in_order_verify_at_location(const pfstest_pg_ptr char *file, int line,
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
