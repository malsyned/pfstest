#include "pfstest-matchers.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "pfstest-core.h"
#include "pfstest-values.h"
#include "pfstest-alloc.h"

/* is */

static void is_printer(pfstest_matcher_t *matcher,
                       pfstest_reporter_t *reporter)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);

    pfstest_value_print(expected, reporter);
}

static pfstest_bool is_test(pfstest_matcher_t *matcher,
                            pfstest_value_t *actual_value)
{
    pfstest_value_t *expected_value =
        (pfstest_value_t *)pfstest_matcher_data(matcher);

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
    return pfstest_matcher_new(is_printer, is_test, expected);
}

/* matches_the_pg_string */

static void matches_the_pg_string_printer(
    pfstest_matcher_t *matcher, pfstest_reporter_t *reporter)
{
    const pfstest_pg_ptr char **sp =
        (const pfstest_pg_ptr char **)pfstest_matcher_data(matcher);
    const pfstest_pg_ptr char *expected = *sp;
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
    const pfstest_pg_ptr char **sp =
        (const pfstest_pg_ptr char **)pfstest_matcher_data(matcher);
    const pfstest_pg_ptr char *expected = *sp;

    const char *actual = (const char *)pfstest_value_data(actual_value);

    return (0 == pfstest_strcmp_pg(actual, expected));
}

pfstest_matcher_t *pfstest_matches_the_pg_string(
    const pfstest_pg_ptr char *s)
{
    const pfstest_pg_ptr char **sp = pfstest_alloc(sizeof(*sp));
    *sp = s;

    /* The cast to (void *) in the third argument is to work around a
     * compiler bug in VC++:

     * https://developercommunity.visualstudio.com/content/problem/390711/c-compiler-incorrect-propagation-of-const-qualifie.html
     */
    return pfstest_matcher_new(matches_the_pg_string_printer,
                               matches_the_pg_string_test,
                               (void *)sp);
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
    return pfstest_matcher_new(is_anything_printer, is_anything_test, NULL);
}

/* int_members_match */

struct submatcher
{
    pfstest_list_node_t node;
    pfstest_matcher_t *matcher;
};

static void int_members_match_printer(pfstest_matcher_t *matcher,
                                      pfstest_reporter_t *reporter)
{
    pfstest_list_t *submatchers = pfstest_matcher_data(matcher);
    struct submatcher *submatcher;

    pfstest_reporter_print_pg_str(reporter, pfstest_pg_str("{ "));

    pfstest_list_iter (submatcher, submatchers) {
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
    pfstest_list_t *submembers = pfstest_matcher_data(matcher);
    pfstest_value_t **boxed_values = reboxer(actual_value);

    return submatchers_match_value_array(submembers,
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
    va_list ap;
    pfstest_list_t *submatchers;

    va_start(ap, first);
    submatchers = package_all_matcher_args(first, ap);
    va_end(ap);

    return pfstest_matcher_new(
        int_members_match_printer, int_members_match_test, submatchers);
}
