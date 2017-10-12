#include "pfstest-matchers.h"

#include <stdlib.h>
#include <stdarg.h>

#include "pfstest-platform.h"
#include "pfstest-values.h"
#include "pfstest-equality.h"
#include "pfstest-alloc.h"
#include "pfstest-list.h"

/* is_the_short */

pfstest_matcher_t *pfstest_is_the_short(short s)
{
    return pfstest_matcher_new(pfstest_equality_printer,
                               pfstest_equality_test,
                               the_short(s));
}

/* is_the_ushort */

pfstest_matcher_t *pfstest_is_the_ushort(unsigned short s)
{
    return pfstest_matcher_new(pfstest_equality_printer,
                               pfstest_equality_test,
                               the_ushort(s));
}

/* is_the_int */

pfstest_matcher_t *pfstest_is_the_int(int i)
{
    return pfstest_matcher_new(pfstest_equality_printer,
                               pfstest_equality_test,
                               the_int(i));
}

/* is_the_uint */

pfstest_matcher_t *pfstest_is_the_uint(unsigned int u)
{
    return pfstest_matcher_new(pfstest_equality_printer,
                               pfstest_equality_test,
                               the_uint(u));
}

/* is_the_enum */

pfstest_matcher_t *pfstest_is_the_enum(int e,
                                       const pfstest_nv_ptr char **name_map)
{
    return pfstest_matcher_new(pfstest_equality_printer,
                               pfstest_equality_test,
                               the_enum(e, name_map));
}

/* is_the_bool */

pfstest_matcher_t *pfstest_is_the_bool(bool b)
{
    return pfstest_matcher_new(pfstest_equality_printer,
                               pfstest_equality_test,
                               the_bool(b));
}

/* is_the_char */

pfstest_matcher_t *pfstest_is_the_char(char c)
{
    return pfstest_matcher_new(pfstest_equality_printer,
                               pfstest_equality_test,
                               the_char(c));
}

/* is_the_string */

pfstest_matcher_t *pfstest_is_the_string(const char *s)
{
    return pfstest_matcher_new(pfstest_equality_printer,
                               pfstest_equality_test,
                               the_string(s));
}

/* is_the_pointer */

static bool is_the_pointer_test(pfstest_matcher_t *matcher,
                                pfstest_value_t *actual)
{
    pfstest_value_t *expected =
        (pfstest_value_t *)pfstest_matcher_data(matcher);
    const void *expected_pointer = pfstest_value_data(expected);
    const void *actual_pointer = pfstest_value_data(actual);

    return (expected_pointer == actual_pointer);
}

pfstest_matcher_t *pfstest_is_the_pointer(const void *p)
{
    return pfstest_matcher_new(pfstest_equality_printer,
                               is_the_pointer_test,
                               the_pointer(p));
}

/* is_the_memory */

pfstest_matcher_t *pfstest_is_the_memory(const void *m, size_t size)
{
    return pfstest_matcher_new(pfstest_equality_printer,
                               pfstest_equality_test,
                               the_memory(m, size));
}

/* matches_the_nv_string */

static void matches_the_nv_string_printer(
    pfstest_reporter_t *reporter, pfstest_matcher_t *matcher)
{
    const pfstest_nv_ptr char **sp =
        (const pfstest_nv_ptr char **)pfstest_matcher_data(matcher);
    const pfstest_nv_ptr char *expected = *sp;
    char c;

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("the string \""));
    
    while (pfstest_memcpy_nv(&c, expected, sizeof(c)), c) {
        pfstest_reporter_print_escaped_char(reporter, c);
        expected++;
    }

    pfstest_reporter_print_nv_string(reporter, pfstest_nv_string("\""));
}

static bool matches_the_nv_string_test(pfstest_matcher_t *matcher,
                                       pfstest_value_t *actual_value)
{
    const pfstest_nv_ptr char **sp =
        (const pfstest_nv_ptr char **)pfstest_matcher_data(matcher);
    const pfstest_nv_ptr char *expected = *sp;

    const char *actual = (const char *)pfstest_value_data(actual_value);

    return (0 == pfstest_strcmp_nv(actual, expected));
}

pfstest_matcher_t *pfstest_matches_the_nv_string(
    const pfstest_nv_ptr char *s)
{
    const pfstest_nv_ptr char **sp = pfstest_alloc(sizeof(*sp));
    *sp = s;

    /* The cast to (void *) in the third argument is to work around a
     * compiler bug in VC++:

     * https://connect.microsoft.com/VisualStudio/feedback/details/3111046/c-compiler-incorrectly-emits-c4090-warning
     */
    return pfstest_matcher_new(matches_the_nv_string_printer,
                               matches_the_nv_string_test,
                               (void *)sp);
}

/* is_anything */

static void is_anything_printer(pfstest_reporter_t *reporter,
                                pfstest_matcher_t *matcher)
{
    (void)matcher;
    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("anything"));
}

static bool is_anything_test(pfstest_matcher_t *matcher,
                             pfstest_value_t *actual)
{
    (void)matcher;
    (void)actual;
    return true;
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

static void int_members_match_printer(pfstest_reporter_t *reporter,
                                      pfstest_matcher_t *matcher)
{
    pfstest_list_t *submatchers = pfstest_matcher_data(matcher);
    pfstest_list_node_t *submatcher_node;

    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string("{ "));

    pfstest_list_iter (submatcher_node, submatchers) {
        struct submatcher *submatcher = (struct submatcher *)submatcher_node;
        pfstest_matcher_print(reporter, submatcher->matcher);

        if (submatcher_node->next != NULL) {
            pfstest_reporter_print_nv_string(
                reporter, pfstest_nv_string(", "));
        }
    }
    pfstest_reporter_print_nv_string(
        reporter, pfstest_nv_string(" }"));
}

static bool int_members_match_test(pfstest_matcher_t *matcher,
                                   pfstest_value_t *actual_value)
{
    pfstest_list_t *submatchers = pfstest_matcher_data(matcher);
    const int *actual = pfstest_value_data(actual_value);
    size_t i = 0;
    pfstest_list_node_t *submatcher_node;

    pfstest_list_iter (submatcher_node, submatchers) {
        struct submatcher *submatcher = (struct submatcher *)submatcher_node;

        pfstest_value_t *member_value = the_int(actual[i++]);
        pfstest_matcher_t *member_matcher = submatcher->matcher;

        bool match = pfstest_matcher_matches(member_matcher, member_value);
        if (!match)
            return false;
    }
    return true;
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
