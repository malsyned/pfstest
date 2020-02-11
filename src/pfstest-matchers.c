#include "pfstest-matchers.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "pfstest-core.h"
#include "pfstest-values.h"
#include "pfstest-alloc.h"

/* is */

struct is_matcher
{
    pfstest_matcher_t parent;
    pfstest_value_t *expected;
};

static void is_printer(pfstest_matcher_t *matcher,
                       pfstest_reporter_t *reporter)
{
    struct is_matcher *is_matcher = (struct is_matcher *)matcher;
    pfstest_value_t *expected = is_matcher->expected;

    pfstest_value_print(expected, reporter);
}

static pfstest_bool is_test(pfstest_matcher_t *matcher,
                            pfstest_value_t *actual_value)
{
    struct is_matcher *is_matcher = (struct is_matcher *)matcher;
    pfstest_value_t *expected_value = is_matcher->expected;

    const void *expected = pfstest_value_data(expected_value);
    size_t expected_size = pfstest_value_size(expected_value);

    const void *actual = pfstest_value_data(actual_value);
    size_t actual_size = pfstest_value_size(actual_value);

    /* If someone is comparing two different-sized blocks of memory in
     * an assertion, that's almost certainly a bug in the tests, not
     * in the production code. Since we're carrying around the size
     * for printing purposes anyway, we might as well check for and
     * catch this mistake. */
    if (expected_size != actual_size)
    {
        /* However, if actual_size is 0, someone is comparing a memory
         * block with a value created with something like
         * the_pointer(). We allow this, especially because auto-mocks
         * are dumb and use the_pointer for everything. The trade-off
         * for this is that the_pointer_printer will print only the
         * address of the actual value. mock verifiers should never
         * print argument values for anything anyway, because the data
         * they'd be printing might be stack data that's gone out of
         * scope by the time the verifier is run. */
        if (actual_size != 0)
            return pfstest_false;
    }

    if (expected_size == 0)
        return (expected == actual);
    else
        return (0 == memcmp(expected, actual, expected_size));
}

pfstest_matcher_t *pfstest_is(pfstest_value_t *expected)
{
    struct is_matcher *m = pfstest_alloc(sizeof(*m));
    pfstest_matcher_init((pfstest_matcher_t *)m, is_printer, is_test);
    m->expected = expected;

    return (pfstest_matcher_t *)m;
}

/* matches_the_pg_string */

struct the_pg_string_matcher
{
    pfstest_matcher_t parent;
    const pfstest_pg_ptr char **sp;
};

static void matches_the_pg_string_printer(
    pfstest_matcher_t *matcher, pfstest_reporter_t *reporter)
{
    struct the_pg_string_matcher *m =
        (struct the_pg_string_matcher *)matcher;
    const pfstest_pg_ptr char *expected = *m->sp;
    char c;

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("the string \""));
    
    while (pfstest_memcpy_pg(&c, expected, sizeof(c)), c) {
        pfstest_reporter_print_escaped_char(reporter, c);
        expected++;
    }

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("\""));
}

static pfstest_bool matches_the_pg_string_test(pfstest_matcher_t *matcher,
                                               pfstest_value_t *actual_value)
{
    struct the_pg_string_matcher *m =
        (struct the_pg_string_matcher *)matcher;
    const pfstest_pg_ptr char *expected = *m->sp;

    const char *actual = (const char *)pfstest_value_data(actual_value);

    return (0 == pfstest_strcmp_pg(actual, expected));
}

pfstest_matcher_t *pfstest_matches_the_pg_string(
    const pfstest_pg_ptr char *s)
{
    struct the_pg_string_matcher *m = pfstest_alloc(sizeof(*m));
    pfstest_matcher_init((pfstest_matcher_t *)m,
                         matches_the_pg_string_printer,
                         matches_the_pg_string_test);
    m->sp = pfstest_alloc(sizeof(*m->sp));
    *m->sp = s;
    return (pfstest_matcher_t *)m;
}

/* is_anything */

static void is_anything_printer(pfstest_matcher_t *matcher,
                                pfstest_reporter_t *reporter)
{
    (void)matcher;
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("anything"));
}

static pfstest_bool is_anything_test(pfstest_matcher_t *matcher,
                                     pfstest_value_t *actual)
{
    (void)matcher;
    (void)actual;
    return pfstest_true;
}

pfstest_matcher_t *_pfstest_is_anything(void)
{
    return pfstest_matcher_new(is_anything_printer, is_anything_test);
}

/* int_members_match */

struct submatcher
{
    pfstest_list_node_t node;
    pfstest_matcher_t *matcher;
};

struct members_matcher
{
    pfstest_matcher_t parent;
    pfstest_list_t *submatchers;
};

static void int_members_match_printer(pfstest_matcher_t *matcher,
                                      pfstest_reporter_t *reporter)
{
    struct members_matcher *members_matcher =
        (struct members_matcher *)matcher;
    struct submatcher *submatcher;

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("{ "));

    pfstest_list_iter (submatcher, members_matcher->submatchers) {
        pfstest_matcher_print(submatcher->matcher, reporter);

        if (((pfstest_list_node_t *)submatcher)->next != NULL) {
            pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(", "));
        }
    }
    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str(" }"));
}

static pfstest_bool submatchers_match_value_array(
    pfstest_list_t *submatchers, pfstest_value_t **values)
{
    struct submatcher *submatcher;

    pfstest_list_iter (submatcher, submatchers) {
        pfstest_value_t *member_value = *values++;
        pfstest_matcher_t *member_matcher = submatcher->matcher;

        pfstest_bool match =
            pfstest_matcher_matches(member_matcher, member_value);
        if (!match)
            return pfstest_false;
    }
    return pfstest_true;
}

#define primitive_array_boxer_define(fname, type, boxer)            \
    static pfstest_value_t **fname(pfstest_value_t *actual_value)   \
    {                                                               \
        size_t actual_count = (pfstest_value_size(actual_value)     \
                               / sizeof(type));                     \
        const type *actual = pfstest_value_data(actual_value);      \
        pfstest_value_t **actual_values =                           \
            pfstest_alloc(sizeof(*actual_values) * actual_count);   \
        size_t i;                                                   \
                                                                    \
        for (i = 0; i < actual_count; i++) {                        \
            actual_values[i] = boxer(actual[i]);                    \
        }                                                           \
                                                                    \
        return actual_values;                                       \
    } _pfstest_expect_semicolon

primitive_array_boxer_define(box_int_members, int, the_int);

static pfstest_bool members_match_test(
    pfstest_matcher_t *matcher, pfstest_value_t *actual_value,
    pfstest_value_t **(*reboxer)(pfstest_value_t *value))
{
    struct members_matcher *members_matcher =
        (struct members_matcher *)matcher;
    pfstest_value_t **boxed_values = reboxer(actual_value);

    return submatchers_match_value_array(members_matcher->submatchers,
                                         boxed_values);
}

static pfstest_bool int_members_match_test(pfstest_matcher_t *matcher,
                                           pfstest_value_t *actual_value)
{
    return members_match_test(matcher, actual_value, box_int_members);
}

static pfstest_list_t *package_all_matcher_args(
    pfstest_matcher_t *first_arg, va_list ap)
{
    pfstest_list_t *submatchers = pfstest_alloc(sizeof(*submatchers));
    pfstest_matcher_t *current_arg;
    struct submatcher *submatcher;

    pfstest_list_reset(submatchers);
    for (current_arg = first_arg;
         current_arg != NULL;
         current_arg = va_arg(ap, pfstest_matcher_t *))
    {
        submatcher = pfstest_alloc(sizeof(*submatcher));
        pfstest_list_node_init((pfstest_list_node_t *)submatcher);
        submatcher->matcher = current_arg;
        pfstest_list_append(submatchers, (pfstest_list_node_t *)submatcher);
    }

    return submatchers;
}

pfstest_matcher_t *pfstest_int_members_match(pfstest_matcher_t *first,...)
{
    struct members_matcher *members_matcher =
        pfstest_alloc(sizeof(*members_matcher));
    va_list ap;

    pfstest_matcher_init((pfstest_matcher_t *)members_matcher,
                         int_members_match_printer, int_members_match_test);

    va_start(ap, first);
    members_matcher->submatchers = package_all_matcher_args(first, ap);
    va_end(ap);

    return (pfstest_matcher_t *)members_matcher;
}
