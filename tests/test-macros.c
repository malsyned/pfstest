#include "pfstest.h"

#define stringify(x) #x
#define estringify(x) stringify(x)

/* These tests use what would be considered a lot of RAM by the
 * standards of small embedded MCUs, and the output of the macros is
 * very platform- and compiler-specific, so these tests are only run
 * on platforms similar enough to the original development platform
 * and likely to have RAM measured in MiB or better. */
#if (defined(__GNUC__)                                  \
     && (defined(__x86_64__)                            \
         || defined(__i386__)                           \
         || defined(__linux__)))

/* Many of the string literals in this file are longer than the ISO
 * C89 standard of 509 characters */
#pragma GCC diagnostic ignored "-Woverlength-strings"

/* Some of these macros make use of NULL from stdlib.h but there are
 * many equivalent ways to define NULL and these tests shouldn't be
 * sensitive to changes in the definition. */
#define s_NULL estringify(NULL)

test(pfstest_macro)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "("

        "__attribute__((__constructor__)) "
        "static void __pfstest_init__a_test(void) "
        "{ "
        """""do { "
        """""""""extern pfstest_t a_test[]; "
        """""""""_pfstest_register_test(a_test); "
        """""} while (0); "
        "} "
        "struct _pfstest_expect_semicolon_stuct; "
        ""
        "static const _pfstest_fixture_hookp __pfstest_fixture_hookp__setup_hook; "
        "static const _pfstest_fixture_hookp __pfstest_fixture_hookp__teardown_hook; "
        ""
        "static void __pfstest__a_test(void); "
        "static const char __pfstest_name__a_test[] = \"a_test\"; "
        "static const char __pfstest_file__a_test[] = \"test-minimal.c\"; "
        "static const _pfstest_test_pg_t __pfstest_pg__a_test = { "
        """""__pfstest_name__a_test, "
        """""__pfstest_file__a_test, 4747, "
        """""0, "
        """""&__pfstest_fixture_hookp__setup_hook, "
        """""&__pfstest_fixture_hookp__teardown_hook, "
        """""__pfstest__a_test, "
        "}; "
        "pfstest_t a_test[1] = {{ {" s_NULL "}, &__pfstest_pg__a_test"
        "}}; "
        ""
        "static void __pfstest__a_test(void)"

        ")"
        );

#undef __PFSTEST_LINE__
#undef __PFSTEST_FILE__
#define __PFSTEST_LINE__ 4747
#define __PFSTEST_FILE__ "test-minimal.c"
    const char *actual = estringify((test(a_test)));
#undef __PFSTEST_LINE__
#undef __PFSTEST_FILE__
#define __PFSTEST_LINE__ __LINE__
#define __PFSTEST_FILE__ __FILE__

    assert_that("the pfstest() macro expands correctly",
                the_string(actual), matches_the_pg_string(expected));
}

test(setup_macro)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "("

        "static void __pfstest_fixture__setup_hook(void); "
        "static const _pfstest_fixture_hookp __pfstest_fixture_hookp__setup_hook = __pfstest_fixture__setup_hook; "
        "static void __pfstest_fixture__setup_hook(void)"

        ")"
        );

    const char *actual = estringify((setup()));

    assert_that("the pfstest_setup() macro expands correctly",
                the_string(actual), matches_the_pg_string(expected));
}

test(teardown_macro)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "("

        "static void __pfstest_fixture__teardown_hook(void); "
        "static const _pfstest_fixture_hookp __pfstest_fixture_hookp__teardown_hook = __pfstest_fixture__teardown_hook; "
        "static void __pfstest_fixture__teardown_hook(void)"

        ")"
        );

    const char *actual = estringify((teardown()));

    assert_that("the pfstest_teardown() macro expands correctly",
                the_string(actual), matches_the_pg_string(expected));
}

test(plugin_define_macro)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "("

        "static const char __pfstest_plugin_name__a_plugin[] = \"a_plugin\"; "
        "static const _pfstest_plugin_pg_t __pfstest_plugin_pg__a_plugin = "
        "{"
        """""__pfstest_plugin_name__a_plugin, "
        """""{setup_cb, checks_cb, teardown_cb}"
        "}; "
        "pfstest_plugin_t a_plugin[1] = {{ "
        """""{" s_NULL "}, &__pfstest_plugin_pg__a_plugin "
        "}}"

        ")"
        );

    const char *actual = estringify(
        (pfstest_plugin_define(a_plugin, setup_cb, checks_cb, teardown_cb)));

    assert_that("the pfstest_plugin_define() macro expands correctly",
                the_string(actual), matches_the_pg_string(expected));
}

test(plugin_autoload_macro)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "("

        "__attribute__((__constructor__)) "
        "static void __pfstest_plugin_init__a_plugin(void) "
        "{ "
        """""do { "
        """""""""extern pfstest_plugin_t a_plugin[]; "
        """""""""_pfstest_register_plugin(a_plugin); "
        """""} while (0); "
        "} "
        "struct _pfstest_expect_semicolon_stuct"

        ")"
        );

    const char *actual = estringify((pfstest_plugin_autoload(a_plugin)));

    assert_that("the pfstest_plugin_autoload() macro expands correctly",
                the_string(actual), matches_the_pg_string(expected));
}

test(mock_define_macro)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "("

        "static const char __pfstest_mock_func_name__mock_func[] = \"func\"; "
        "const pfstest_mock_t mock_func[1] = {{ "
        "__pfstest_mock_func_name__mock_func, 47}}"

        ")"
        );

    const char *actual = estringify(
        (pfstest_mock_define(mock_func, "func", 47)));

    assert_that("the pfstest_mock_define() macro expands correctly",
                the_string(actual), matches_the_pg_string(expected));
}

test(mock_declare_macro)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "("

        "extern const pfstest_mock_t mock_func[]"

        ")"
        );

    const char *actual = estringify(
        (pfstest_mock_declare(mock_func)));

    assert_that("the pfstest_mock_declare() macro expands correctly",
                the_string(actual), matches_the_pg_string(expected));
}

#endif /* PFSTEST_PLATFORM == PFSTEST_PLATFORM_GCC */
