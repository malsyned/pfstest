/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest-fp.h"

#include "pfstest-alloc.h"

static void (*pfstest_fp_printer)(double, pfstest_reporter_t *) = NULL;

void pfstest_fp_set_printer(void (*printer)(double, pfstest_reporter_t *))
{
    pfstest_fp_printer = printer;
}

static void the_float_printer(pfstest_value_t *value,
                              pfstest_reporter_t *reporter)
{
    float n = *(const float *)pfstest_value_data(value);
    if (pfstest_fp_printer) {
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the float "));
        pfstest_fp_printer((double)n, reporter);
    } else {
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("a float"));
    }
}

pfstest_value_t *pfstest_the_float(float n)
{
    float *data = pfstest_alloc(sizeof(n));
    *data = n;

    return pfstest_value_new(the_float_printer, data, sizeof(n), NULL);
}

struct float_within_args
{
    float expected;
    float tolerance;
};

static void close_to_float_printer(pfstest_matcher_t *matcher,
                                   pfstest_reporter_t *reporter)
{
    struct float_within_args *args = pfstest_matcher_data(matcher);

    if (pfstest_fp_printer) {
        pfstest_reporter_print_pg_str(reporter,
                                      pfstest_pg_str("a float within "));
        pfstest_fp_printer((double)args->tolerance, reporter);
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(" of "));
        pfstest_fp_printer((double)args->expected, reporter);
    } else {
        pfstest_reporter_print_pg_str(
            reporter, pfstest_pg_str("a float near the expected value"));
    }
}

static pfstest_bool close_to_float_test(pfstest_matcher_t *matcher,
                                        pfstest_value_t *actual)
{
    struct float_within_args *args = pfstest_matcher_data(matcher);
    float expected = args->expected;
    float tolerance = args->tolerance;
    float n = *(const float *)pfstest_value_data(actual);

    float delta = expected > n ? expected - n : n - expected;

    return (delta <= tolerance);
}

pfstest_matcher_t *pfstest_close_to_float(float n, float tolerance)
{
    struct float_within_args *args = pfstest_alloc(sizeof(*args));
    args->expected = n;
    args->tolerance = tolerance >= 0.0f ? tolerance : -tolerance;

    return pfstest_matcher_new(close_to_float_printer,
                               close_to_float_test,
                               args);
}

static pfstest_bool floats_equal(float a, float b)
{
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wfloat-equal"
#endif
    return a == b;
#ifdef __GNUC__
#pragma GCC diagnostic error "-Wfloat-equal"
#endif
}

static void an_inf_float_printer(pfstest_matcher_t *matcher,
                                 pfstest_reporter_t *reporter)
{
    (void)matcher;

    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("an inf float"));
}

static pfstest_bool an_inf_float_test(pfstest_matcher_t *matcher,
                                      pfstest_value_t *actual)
{
    float n = *(const float *)pfstest_value_data(actual);
    (void)matcher;

    return floats_equal(n, 1.0f/0.0f);
}

pfstest_matcher_t *pfstest_an_inf_float(void)
{
    return pfstest_matcher_new(an_inf_float_printer, an_inf_float_test,
                               NULL);
}

static void a_neg_inf_float_printer(pfstest_matcher_t *matcher,
                                    pfstest_reporter_t *reporter)
{
    (void)matcher;

    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("a -inf float"));
}

static pfstest_bool a_neg_inf_float_test(pfstest_matcher_t *matcher,
                                         pfstest_value_t *actual)
{
    float n = *(const float *)pfstest_value_data(actual);
    (void)matcher;

    return floats_equal(n, -1.0f/0.0f);
}

pfstest_matcher_t *pfstest_a_neg_inf_float(void)
{
    return pfstest_matcher_new(a_neg_inf_float_printer,
                               a_neg_inf_float_test, NULL);
}

static void a_nan_float_printer(pfstest_matcher_t *matcher,
                                pfstest_reporter_t *reporter)
{
    (void)matcher;
    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("a nan float"));
}

static pfstest_bool a_nan_float_test(pfstest_matcher_t *matcher,
                                     pfstest_value_t *actual)
{
    float n = *(const float *)pfstest_value_data(actual);
    (void)matcher;

    return !floats_equal(n, n);
}

pfstest_matcher_t *pfstest_a_nan_float(void)
{
    return pfstest_matcher_new(a_nan_float_printer,
                               a_nan_float_test, NULL);
}

static void the_double_printer(pfstest_value_t *value,
                               pfstest_reporter_t *reporter)
{
    double n = *(const double *)pfstest_value_data(value);
    if (pfstest_fp_printer) {
        pfstest_reporter_print_pg_str(reporter,
                                      pfstest_pg_str("the double "));
        pfstest_fp_printer((double)n, reporter);
    } else {
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("a double"));
    }
}

pfstest_value_t *pfstest_the_double(double n)
{
    double *data = pfstest_alloc(sizeof(n));
    *data = n;

    return pfstest_value_new(the_double_printer, data, sizeof(n), NULL);
}

struct double_within_args
{
    double expected;
    double tolerance;
};

static void close_to_double_printer(pfstest_matcher_t *matcher,
                                    pfstest_reporter_t *reporter)
{
    struct double_within_args *args = pfstest_matcher_data(matcher);

    if (pfstest_fp_printer) {
        pfstest_reporter_print_pg_str(reporter,
                                      pfstest_pg_str("a double within "));
        pfstest_fp_printer((double)args->tolerance, reporter);
        pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(" of "));
        pfstest_fp_printer((double)args->expected, reporter);
    } else {
        pfstest_reporter_print_pg_str(
            reporter, pfstest_pg_str("a double near the expected value"));
    }
}

static pfstest_bool close_to_double_test(pfstest_matcher_t *matcher,
                                         pfstest_value_t *actual)
{
    struct double_within_args *args = pfstest_matcher_data(matcher);
    double expected = args->expected;
    double tolerance = args->tolerance;
    double n = *(const double *)pfstest_value_data(actual);

    double delta = expected > n ? expected - n : n - expected;

    return (delta <= tolerance);
}

pfstest_matcher_t *pfstest_close_to_double(double n, double tolerance)
{
    struct double_within_args *args = pfstest_alloc(sizeof(*args));
    args->expected = n;
    args->tolerance = tolerance >= 0.0f ? tolerance : -tolerance;

    return pfstest_matcher_new(close_to_double_printer,
                               close_to_double_test,
                               args);
}

static pfstest_bool doubles_equal(double a, double b)
{
#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wfloat-equal"
#endif
    return a == b;
#ifdef __GNUC__
#pragma GCC diagnostic error "-Wfloat-equal"
#endif
}

static void an_inf_double_printer(pfstest_matcher_t *matcher,
                                  pfstest_reporter_t *reporter)
{
    (void)matcher;

    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("an inf double"));
}

static pfstest_bool an_inf_double_test(pfstest_matcher_t *matcher,
                                       pfstest_value_t *actual)
{
    double n = *(const double *)pfstest_value_data(actual);
    (void)matcher;

    return doubles_equal(n, 1.0f/0.0f);
}

pfstest_matcher_t *pfstest_an_inf_double(void)
{
    return pfstest_matcher_new(an_inf_double_printer, an_inf_double_test,
                               NULL);
}

static void a_neg_inf_double_printer(pfstest_matcher_t *matcher,
                                     pfstest_reporter_t *reporter)
{
    (void)matcher;

    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("a -inf double"));
}

static pfstest_bool a_neg_inf_double_test(pfstest_matcher_t *matcher,
                                          pfstest_value_t *actual)
{
    double n = *(const double *)pfstest_value_data(actual);
    (void)matcher;

    return doubles_equal(n, -1.0f/0.0f);
}

pfstest_matcher_t *pfstest_a_neg_inf_double(void)
{
    return pfstest_matcher_new(a_neg_inf_double_printer,
                               a_neg_inf_double_test, NULL);
}

static void a_nan_double_printer(pfstest_matcher_t *matcher,
                                 pfstest_reporter_t *reporter)
{
    (void)matcher;
    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("a nan double"));
}

static pfstest_bool a_nan_double_test(pfstest_matcher_t *matcher,
                                      pfstest_value_t *actual)
{
    double n = *(const double *)pfstest_value_data(actual);
    (void)matcher;

    return !doubles_equal(n, n);
}

pfstest_matcher_t *pfstest_a_nan_double(void)
{
    return pfstest_matcher_new(a_nan_double_printer,
                               a_nan_double_test, NULL);
}
