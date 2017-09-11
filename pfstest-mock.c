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

void pfstest_mock_init(void)
{
    dynamic_env_t *new_frame = pfstest_alloc(sizeof(*new_frame));
    new_frame->next = dynamic_env;
    dynamic_env = new_frame;
    
    pfstest_list_reset(&dynamic_env->expectations);
    pfstest_list_reset(&dynamic_env->invocations);
    pfstest_list_reset(&dynamic_env->verifiers);
    pfstest_list_reset(&dynamic_env->default_expectations);
}

void pfstest_mock_finish(void)
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

static int pfstest_mock_arg_count(const pfstest_nv_ptr pfstest_mock_t *mock)
{
    pfstest_mock_t m;
    pfstest_memcpy_nv(&m, mock, sizeof(m));

    return m.arg_count;
}

static const pfstest_nv_ptr char *pfstest_mock_name(
    const pfstest_nv_ptr pfstest_mock_t *mock)
{
    pfstest_mock_t m;
    pfstest_memcpy_nv(&m, mock, sizeof(m));

    return m.name;
}

/* expectation */

#define PFSTEST_EXPECTATION_DO_TIMES_INFINITE -1

static pfstest_expectation_t *pfstest_expectation_new(
    const pfstest_nv_ptr pfstest_mock_t *mock,
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

static void pfstest_expectation_print(pfstest_output_formatter_t *formatter,
                                      pfstest_expectation_t *e)
{
    int i;

    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_mock_name(e->mock));
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string(" with ("));

    for (i = 0; i < pfstest_mock_arg_count(e->mock); i++) {
        pfstest_arg_handler_print(formatter, e->arg_handlers[i]);

        if (i < pfstest_mock_arg_count(e->mock) - 1)
            pfstest_output_formatter_print_nv_string(
                formatter, pfstest_nv_string(", "));
    }
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string(")"));
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
    const pfstest_nv_ptr pfstest_mock_t *mock, ...)
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
    i->mark = false;

    return i;
}

static bool args_match(int arg_count,
                       pfstest_value_t **args,
                       pfstest_arg_handler_t **arg_handlers)
{
    int i;

    /* The default expectation, which should match everything, has
     * NULL arg_handlers */
    if (arg_handlers == NULL) {
        return true;
    }

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

static pfstest_expectation_t *find_default_expectation(
    const pfstest_nv_ptr pfstest_mock_t *mock)
{
    pfstest_list_node_t *node;

    pfstest_list_iter (node, &dynamic_env->default_expectations) {
        pfstest_expectation_t *e = (pfstest_expectation_t *)node;

        if (mock == e->mock) {
            return e;
        }
    }

    return NULL;
}

static pfstest_expectation_t *get_default_expectation(
    const pfstest_nv_ptr pfstest_mock_t *mock)
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
    const pfstest_nv_ptr pfstest_mock_t *mock,
    pfstest_value_t *default_return_value,
    ...)
{
    pfstest_list_node_t *expectation_node;
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

    pfstest_list_iter (expectation_node, &dynamic_env->expectations) {
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
    }
    if (expectation_node == NULL) {
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

void pfstest_run_verifiers(void)
{
    pfstest_list_node_t *verifier_node;

    pfstest_list_iter (verifier_node, &dynamic_env->verifiers) {
        pfstest_verifier_t *v = (pfstest_verifier_t *)verifier_node;

        v->function(v);
    }
}

/* basic verifier */

struct do_verification_printer_args 
{
    pfstest_expectation_t *expectation;
    int invocation_count;
    const pfstest_nv_ptr char *wanted_desc_prefix;
    int wanted_count;
};

static void print_plural(pfstest_output_formatter_t *formatter, int count)
{
    if (count != 1) {
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string("s"));
    }
}

static void wrong_call_count_printer(pfstest_output_formatter_t *formatter,
                                     const void *data)
{
    const struct do_verification_printer_args *args = data;

    if (args->invocation_count == 0) {
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string("Never called "));
        pfstest_expectation_print(formatter, args->expectation);
    } else if (args->invocation_count != 1) {
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string("Wanted "));
        pfstest_expectation_print(formatter, args->expectation);

        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string(" "));
        pfstest_output_formatter_print_nv_string(
            formatter, args->wanted_desc_prefix);
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string(" "));
        pfstest_output_formatter_print_int(
            formatter, args->wanted_count);
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string(" time"));
        print_plural(formatter, args->wanted_count);
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string("\n"));

        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string("Was called "));
        pfstest_output_formatter_print_int(
            formatter, args->invocation_count);
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string(" time"));
        print_plural(formatter, args->invocation_count);
    }
}

void pfstest_verify_at_location(const pfstest_nv_ptr char *file, int line,
                                pfstest_expectation_t *e)
{
    pfstest_verify_times_at_location(file, line, exactly(1), e);
}

struct verify_with_mode_args
{
    const pfstest_nv_ptr char *file;
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

void pfstest_verify_times_at_location(const pfstest_nv_ptr char *file,
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
    pfstest_list_node_t *invocation_node;

    pfstest_list_iter (invocation_node, &dynamic_env->invocations) {
        pfstest_invocation_t *i = (pfstest_invocation_t *)invocation_node;

        if (expectation == i->expectation) {
            i->mark = true;
            invocation_count++;
        }
    }

    return invocation_count;
}

static void fail_wrong_call_count(
    const pfstest_nv_ptr char *file, int line,
    pfstest_expectation_t *expectation,
    int invocation_count,
    const pfstest_nv_ptr char *wanted_desc_prefix,
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

static void do_exactly(const pfstest_nv_ptr char *file, int line,
                       pfstest_verify_mode_t *mode,
                       pfstest_expectation_t *expectation)
{
    int wanted_count = *(int *)mode->data;
    int invocation_count = count_and_mark_invocations(expectation);

    if (invocation_count != wanted_count) {
        fail_wrong_call_count(file, line, expectation,
                              invocation_count,
                              pfstest_nv_string("exactly"), wanted_count);
    }
}

static pfstest_verify_mode_t *counting_mode_new(
    int times,
    void function(const pfstest_nv_ptr char *file, int line,
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

static void do_at_most(const pfstest_nv_ptr char *file, int line,
                       pfstest_verify_mode_t *mode,
                       pfstest_expectation_t *expectation)
{
    int wanted_count = *(int *)mode->data;
    int invocation_count = count_and_mark_invocations(expectation);

    if (invocation_count > wanted_count) {
        fail_wrong_call_count(file, line, expectation,
                              invocation_count,
                              pfstest_nv_string("at most"), wanted_count);
    }
}

pfstest_verify_mode_t *pfstest_at_most(int times)
{
    return counting_mode_new(times, do_at_most);
}

static void do_at_least(const pfstest_nv_ptr char *file, int line,
                        pfstest_verify_mode_t *mode,
                        pfstest_expectation_t *expectation)
{
    int wanted_count = *(int *)mode->data;
    int invocation_count = count_and_mark_invocations(expectation);

    if (invocation_count < wanted_count) {
        fail_wrong_call_count(file, line, expectation,
                              invocation_count,
                              pfstest_nv_string("at least"), wanted_count);
    }
}

pfstest_verify_mode_t *pfstest_at_least(int times)
{
    return counting_mode_new(times, do_at_least);
}

struct no_more_interactions_printer_args
{
    const pfstest_nv_ptr pfstest_mock_t *mock;
};

static void no_more_interactions_printer(
    pfstest_output_formatter_t *formatter, const void *data)
{
    const struct no_more_interactions_printer_args *args = data;

    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("Unexpected interactions with "));
    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_mock_name(args->mock));
}

struct no_more_interactions_args
{
    const pfstest_nv_ptr pfstest_mock_t *mock;
    const pfstest_nv_ptr char *file;
    int line;
};

static void do_verify_no_more_interactions(pfstest_verifier_t *v)
{
    struct no_more_interactions_args *args = v->data;
    
    pfstest_list_node_t *invocation_node;

    pfstest_list_iter (invocation_node, &dynamic_env->invocations) {
        pfstest_invocation_t *i = (pfstest_invocation_t *)invocation_node;
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
    const pfstest_nv_ptr char *file,
    int line,
    const pfstest_nv_ptr pfstest_mock_t *mock)
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
    const pfstest_nv_ptr char *file;
    int line;
};

static void do_verify_no_more_invocations(pfstest_verifier_t *v)
{
    struct no_more_invocations_args *args = v->data;
    pfstest_list_node_t *invocation_node;

    pfstest_list_iter (invocation_node, &dynamic_env->invocations) {
        pfstest_invocation_t *i = (pfstest_invocation_t *)invocation_node;

        if (!i->mark) {
            pfstest_fail_at_location(args->file, args->line,
                                     "Unexpected mock invocations");
        }
    }
}

void pfstest_verify_no_more_invocations_at_location(
    const pfstest_nv_ptr char *file, int line)
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

static void in_order_fail_printer(pfstest_output_formatter_t *formatter,
                                  const void *data)
{
    const struct in_order_fail_printer_args *args = data;

    pfstest_output_formatter_print_nv_string(
        formatter, pfstest_nv_string("Not called in order: "));
    pfstest_expectation_print(formatter, args->expectation);
    if (args->prev_expectation != NULL) {
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string("\n"));
        pfstest_output_formatter_print_nv_string(
            formatter, pfstest_nv_string("Expected after: "));
        pfstest_expectation_print(formatter, args->prev_expectation);
    }
}

struct in_order_expectation
{
    pfstest_list_node_t node;
    const pfstest_nv_ptr char *file;
    int line;
    pfstest_expectation_t *expectation;
};

static void do_in_order_verification(pfstest_verifier_t *v)
{
    in_order_t *order = v->data;
    pfstest_list_node_t *invocation_node;
    pfstest_list_node_t *in_order_expectation_node =
        pfstest_list_head(&order->expectations);
    pfstest_expectation_t *prev_expectation = NULL;
    struct in_order_expectation *in_order_expectation;

    pfstest_list_iter (invocation_node, &dynamic_env->invocations) {
        pfstest_invocation_t *invocation;

        if (in_order_expectation_node == NULL)
            break;

        in_order_expectation =
            (struct in_order_expectation *)in_order_expectation_node;
        invocation = (pfstest_invocation_t *)invocation_node;

        if (in_order_expectation->expectation == invocation->expectation) {
            invocation->mark = true;
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

void pfstest_in_order_verify_at_location(const pfstest_nv_ptr char *file, int line,
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
