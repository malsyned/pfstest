#include <limits.h>

#include "pfstest.h"
#include "capture-output.h"

before_tests(set_up_values_and_matchers)
{
    capture_output_init();
}

test(should_match_shorts)
{
    assert_that("same shorts pass", the_short(5), is_the_short(5));
}

pfstest_case(assert_different_shorts)
{
    assert_that("", the_short(5), is_the_short(6));
}

test(should_fail_on_different_shorts)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the short 6\n"
        "Actual:   the short 5");

    capture_test_results(assert_different_shorts);

    assert_that("different shorts fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_ushorts)
{
    assert_that("same ushorts pass", the_ushort(5), is_the_ushort(5));
}

pfstest_case(assert_different_ushorts)
{
    assert_that("", the_ushort(5), is_the_ushort(6));
}

test(should_fail_on_different_ushorts)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the ushort 6\n"
        "Actual:   the ushort 5");

    capture_test_results(assert_different_ushorts);

    assert_that("different ushorts fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_ints)
{
    assert_that("same ints pass", the_int(5), is_the_int(5));
}

pfstest_case(assert_different_ints)
{
    assert_that("", the_int(-6), is_the_int(6));
}

test(should_fail_on_different_ints)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the int 6\n"
        "Actual:   the int -6");

    capture_test_results(assert_different_ints);

    assert_that("different ints fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_uints)
{
    assert_that("same uints pass",
                the_uint(5), is_the_uint(5));
}

pfstest_case(assert_different_uints)
{
    assert_that("", the_uint(5), is_the_uint(6));
}

test(should_fail_on_different_uints)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the uint 6\n"
        "Actual:   the uint 5");

    capture_test_results(assert_different_uints);

    assert_that("different uints fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_bools)
{
    assert_that("same bools pass",
                the_bool(true), is_the_bool(true));
}

pfstest_case(assert_different_bools)
{
    assert_that("", the_bool(true), is_the_bool(false));
}

test(should_fail_on_different_bools)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the bool <false>\n"
        "Actual:   the bool <true>");
    
    capture_test_results(assert_different_bools);

    assert_that("different bools fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_chars)
{
    assert_that("same chars pass", the_char('c'), is_the_char('c'));
}

pfstest_case(assert_different_chars)
{
    assert_that("", the_char('c'), is_the_char('d'));
}

test(should_fail_on_different_chars)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the char 'd'\n"
        "Actual:   the char 'c'");

    capture_test_results(assert_different_chars);

    assert_that("different chars fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

pfstest_case(assert_special_chars)
{
    assert_that("", the_char('\n'), is_the_char('n'));
}

test(the_char_should_escape_special_chars_in_failure_messages)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the char 'n'\n"
        "Actual:   the char '\\n'");

    capture_test_results(assert_special_chars);

    assert_that("different chars fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_strings)
{
    char s1[] = "foo";
    char s2[] = "foo";

    assert_that("same strings pass", the_string(s1), is_the_string(s2));
}

pfstest_case(assert_different_strings)
{
    char s1[] = "foo";
    char s2[] = "bar";
    assert_that("", the_string(s1), is_the_string(s2));
}

test(should_fail_on_different_strings)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the string \"bar\"\n"
        "Actual:   the string \"foo\"");

    capture_test_results(assert_different_strings);

    assert_that("different strings fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

pfstest_case(assert_shorter_actual_string)
{
    char s1[] = "foo";
    char s2[] = "foobar";

    assert_that("", the_string(s1), is_the_string(s2));
}

test(should_fail_on_shorter_actual_string)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the string \"foobar\"\n"
        "Actual:   the string \"foo\"");

    capture_test_results(assert_shorter_actual_string);

    assert_that("shorter actual string fails",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

pfstest_case(assert_shorter_expected_string)
{
    char s1[] = "foobar";
    char s2[] = "foo";

    assert_that("", the_string(s1), is_the_string(s2));
}

test(should_fail_on_shorter_expected_string)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the string \"foo\"\n"
        "Actual:   the string \"foobar\"");

    capture_test_results(assert_shorter_expected_string);

    assert_that("shorter expected string fails",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

pfstest_case(assert_special_strings)
{
    char s1[] = "\a\b\f\n\r";
    char s2[] = "\t\v\\\"\?";
    assert_that("", the_string(s1), is_the_string(s2));
}

test(the_string_should_escape_special_chars_in_failure_messages)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the string \"\\t\\v\\\\\\\"\\?\"\n"
        "Actual:   the string \"\\a\\b\\f\\n\\r\"");

    capture_test_results(assert_special_strings);

    assert_that("different strings fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

#ifdef __18CXX
/* All of the strings in this file overran 256 bytes of idata */
#pragma idata values_and_matchers_2
#endif

test(should_match_pointers)
{
    /* Use only 16-bit pointers, since these should work on any
     * conceivable platform */
    assert_that("same pointers pass",
                the_pointer((void *)0xbeef), is_the_pointer((void *)0xbeef));
}

pfstest_case(assert_different_pointers)
{
    assert_that("",
                the_pointer((void *)0xdead), is_the_pointer((void *)0xbeef));
}

test(should_fail_on_different_pointers)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the pointer <0xbeef>\n"
        "Actual:   the pointer <0xdead>");

    capture_test_results(assert_different_pointers);

    assert_that("different pointers fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

pfstest_case(assert_pointer_against_null)
{
    assert_that("", the_pointer((void *)0xdead), is_the_pointer(NULL));
}

test(should_print_null_pointers_specially)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the pointer <NULL>\n"
        "Actual:   the pointer <0xdead>");

    capture_test_results(assert_pointer_against_null);

    assert_that("NULL pointers are printed specially",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_memory)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_that("same memory passes",
                the_memory(actual, sizeof(actual)),
                is_the_memory(expected, sizeof(expected)));
}

pfstest_case(assert_different_memory)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 4, 4, 5};

    assert_that("",
                the_memory(actual, sizeof(actual)),
                is_the_memory(expected, sizeof(expected)));
}

test(should_fail_on_different_memory)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the memory {0x01, 0x02, 0x04, 0x04, 0x05}\n"
        "Actual:   the memory {0x01, 0x02, 0x03, 0x04, 0x05}");

    capture_test_results(assert_different_memory);

    assert_that("different memory fails",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

pfstest_case(assert_longer_memory)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_that("",
                the_memory(actual, sizeof(actual)),
                is_the_memory(expected, sizeof(expected) - 1));
}

test(should_fail_on_longer_memory)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the memory {0x01, 0x02, 0x03, 0x04}\n"
        "Actual:   the memory {0x01, 0x02, 0x03, 0x04, 0x05}");

    capture_test_results(assert_longer_memory);

    assert_that("longer memory fails",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

pfstest_case(assert_shorter_memory)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_that("",
                the_memory(actual, sizeof(actual) - 1),
                is_the_memory(expected, sizeof(expected)));
}

test(should_fail_on_shorter_memory)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the memory {0x01, 0x02, 0x03, 0x04, 0x05}\n"
        "Actual:   the memory {0x01, 0x02, 0x03, 0x04}");

    capture_test_results(assert_shorter_memory);

    assert_that("shorter memory fails",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_memory_with_pointer)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_that("is_the_memory matches against the_pointer values",
                the_pointer(actual),
                is_the_memory(expected, sizeof(expected)));
}

test(should_match_primitive_with_pointer)
{
    int actual = 5;
    int expected = 5;

    assert_that("is_the_int matches against the_pointer values",
                the_pointer(&actual), is_the_int(expected));
}

pfstest_case(assert_different_data_types)
{
    unsigned char buf[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    
    /* This test should be valid regardless of endianness, since all
     * slices of buf should look like USHRT_MAX, so the actual
     * contents of the data buffer should always match, requiring a
     * check of the sizes. This test may fail on a platform where
     * shorts are 64-bit, but I'm not aware of any such platform. */
    assert_that("",
                the_memory(buf, sizeof(buf)), is_the_ushort(USHRT_MAX));
}

test(should_detect_data_type_mismatches)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the ushort 65535\n"
        "Actual:   the memory {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}"
        );

    capture_test_results(assert_different_data_types);

    assert_that("different data types fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}
