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

    return pfstest_value_new(the_float_printer, data, sizeof(n));
}

struct float_within_args
{
    float expected;
    float tolerance;
};

static void is_a_float_within_printer(pfstest_matcher_t *matcher,
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

static pfstest_bool is_a_float_within_test(pfstest_matcher_t *matcher,
                                           pfstest_value_t *actual)
{
    struct float_within_args *args = pfstest_matcher_data(matcher);
    float expected = args->expected;
    float tolerance = args->tolerance;
    float n = *(const float *)pfstest_value_data(actual);

    float delta = expected > n ? expected - n : n - expected;

    return (delta <= tolerance);
}

pfstest_matcher_t *pfstest_is_a_float_within(float n, float tolerance)
{
    struct float_within_args *args = pfstest_alloc(sizeof(*args));
    args->expected = n;
    args->tolerance = tolerance >= 0.0f ? tolerance : -tolerance;

    return pfstest_matcher_new(is_a_float_within_printer,
                               is_a_float_within_test,
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

static void is_float_inf_printer(pfstest_matcher_t *matcher,
                                 pfstest_reporter_t *reporter)
{
    (void)matcher;

    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("the float inf"));
}

static pfstest_bool is_float_inf_test(pfstest_matcher_t *matcher,
                                      pfstest_value_t *actual)
{
    float n = *(const float *)pfstest_value_data(actual);
    (void)matcher;

    return floats_equal(n, 1.0f/0.0f);
}

pfstest_matcher_t *_pfstest_is_float_inf(void)
{
    return pfstest_matcher_new(is_float_inf_printer, is_float_inf_test,
                               NULL);
}

static void is_float_neg_inf_printer(pfstest_matcher_t *matcher,
                                     pfstest_reporter_t *reporter)
{
    (void)matcher;

    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("the float -inf"));
}

static pfstest_bool is_float_neg_inf_test(pfstest_matcher_t *matcher,
                                          pfstest_value_t *actual)
{
    float n = *(const float *)pfstest_value_data(actual);
    (void)matcher;

    return floats_equal(n, -1.0f/0.0f);
}

pfstest_matcher_t *_pfstest_is_float_neg_inf(void)
{
    return pfstest_matcher_new(is_float_neg_inf_printer,
                               is_float_neg_inf_test, NULL);
}

static void is_float_nan_printer(pfstest_matcher_t *matcher,
                                 pfstest_reporter_t *reporter)
{
    (void)matcher;
    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("the float nan"));
}

static pfstest_bool is_float_nan_test(pfstest_matcher_t *matcher,
                                      pfstest_value_t *actual)
{
    float n = *(const float *)pfstest_value_data(actual);
    (void)matcher;

    return !floats_equal(n, n);
}

pfstest_matcher_t *_pfstest_is_float_nan(void)
{
    return pfstest_matcher_new(is_float_nan_printer,
                               is_float_nan_test, NULL);
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

    return pfstest_value_new(the_double_printer, data, sizeof(n));
}

struct double_within_args
{
    double expected;
    double tolerance;
};

static void is_a_double_within_printer(pfstest_matcher_t *matcher,
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

static pfstest_bool is_a_double_within_test(pfstest_matcher_t *matcher,
                                            pfstest_value_t *actual)
{
    struct double_within_args *args = pfstest_matcher_data(matcher);
    double expected = args->expected;
    double tolerance = args->tolerance;
    double n = *(const double *)pfstest_value_data(actual);

    double delta = expected > n ? expected - n : n - expected;

    return (delta <= tolerance);
}

pfstest_matcher_t *pfstest_is_a_double_within(double n, double tolerance)
{
    struct double_within_args *args = pfstest_alloc(sizeof(*args));
    args->expected = n;
    args->tolerance = tolerance >= 0.0f ? tolerance : -tolerance;

    return pfstest_matcher_new(is_a_double_within_printer,
                               is_a_double_within_test,
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

static void is_double_inf_printer(pfstest_matcher_t *matcher,
                                  pfstest_reporter_t *reporter)
{
    (void)matcher;

    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("the double inf"));
}

static pfstest_bool is_double_inf_test(pfstest_matcher_t *matcher,
                                       pfstest_value_t *actual)
{
    double n = *(const double *)pfstest_value_data(actual);
    (void)matcher;

    return doubles_equal(n, 1.0f/0.0f);
}

pfstest_matcher_t *_pfstest_is_double_inf(void)
{
    return pfstest_matcher_new(is_double_inf_printer, is_double_inf_test,
                               NULL);
}

static void is_double_neg_inf_printer(pfstest_matcher_t *matcher,
                                      pfstest_reporter_t *reporter)
{
    (void)matcher;

    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("the double -inf"));
}

static pfstest_bool is_double_neg_inf_test(pfstest_matcher_t *matcher,
                                           pfstest_value_t *actual)
{
    double n = *(const double *)pfstest_value_data(actual);
    (void)matcher;

    return doubles_equal(n, -1.0f/0.0f);
}

pfstest_matcher_t *_pfstest_is_double_neg_inf(void)
{
    return pfstest_matcher_new(is_double_neg_inf_printer,
                               is_double_neg_inf_test, NULL);
}

static void is_double_nan_printer(pfstest_matcher_t *matcher,
                                  pfstest_reporter_t *reporter)
{
    (void)matcher;
    pfstest_reporter_print_pg_str(
        reporter, pfstest_pg_str("the double nan"));
}

static pfstest_bool is_double_nan_test(pfstest_matcher_t *matcher,
                                       pfstest_value_t *actual)
{
    double n = *(const double *)pfstest_value_data(actual);
    (void)matcher;

    return !doubles_equal(n, n);
}

pfstest_matcher_t *_pfstest_is_double_nan(void)
{
    return pfstest_matcher_new(is_double_nan_printer,
                               is_double_nan_test, NULL);
}
