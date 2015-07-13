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

static void assert_different_shorts(void)
{
    assert_that("", the_short(5), is_the_short(6));
}

test(should_fail_on_different_shorts)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the short 6\n"
        "Actual: the short 5");

    pfstest_protect_call(assert_different_shorts, &message_spy);

    assert_that("different shorts fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_ushorts)
{
    assert_that("same ushorts pass", the_ushort(5), is_the_ushort(5));
}

static void assert_different_ushorts(void)
{
    assert_that("", the_ushort(5), is_the_ushort(6));
}

test(should_fail_on_different_ushorts)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the ushort 6\n"
        "Actual: the ushort 5");

    pfstest_protect_call(assert_different_ushorts, &message_spy);

    assert_that("different ushorts fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_ints)
{
    assert_that("same ints pass", the_int(5), is_the_int(5));
}

static void assert_different_ints(void)
{
    assert_that("", the_int(-6), is_the_int(6));
}

test(should_fail_on_different_ints)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the int 6\n"
        "Actual: the int -6");

    pfstest_protect_call(assert_different_ints, &message_spy);

    assert_that("different ints fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_uints)
{
    assert_that("same uints pass",
                the_uint(5), is_the_uint(5));
}

static void assert_different_uints(void)
{
    assert_that("", the_uint(5), is_the_uint(6));
}

test(should_fail_on_different_uints)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the uint 6\n"
        "Actual: the uint 5");

    pfstest_protect_call(assert_different_uints, &message_spy);

    assert_that("different uints fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_chars)
{
    assert_that("same chars pass", the_char('c'), is_the_char('c'));
}

static void assert_different_chars(void)
{
    assert_that("", the_char('c'), is_the_char('d'));
}

test(should_fail_on_different_chars)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the char 'd'\n"
        "Actual: the char 'c'");

    pfstest_protect_call(assert_different_chars, &message_spy);

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

static void assert_different_strings(void)
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
        "Actual: the string \"foo\"");

    pfstest_protect_call(assert_different_strings, &message_spy);

    assert_that("different strings fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

static void assert_shorter_actual_string(void)
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
        "Actual: the string \"foo\"");

    pfstest_protect_call(assert_shorter_actual_string, &message_spy);

    assert_that("shorter actual string fails",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

static void assert_shorter_expected_string(void)
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
        "Actual: the string \"foobar\"");

    pfstest_protect_call(assert_shorter_expected_string, &message_spy);

    assert_that("shorter expected string fails",
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

static void assert_different_pointers(void)
{
    assert_that("",
                the_pointer((void *)0xdead), is_the_pointer((void *)0xbeef));
}

test(should_fail_on_different_pointers)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the pointer <0xbeef>\n"
        "Actual: the pointer <0xdead>");

    pfstest_protect_call(assert_different_pointers, &message_spy);

    assert_that("different pointers fail",
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

static void assert_different_memory(void)
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
        "Actual: the memory {0x01, 0x02, 0x03, 0x04, 0x05}");

    pfstest_protect_call(assert_different_memory, &message_spy);

    assert_that("different memory fails",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

static void assert_longer_memory(void)
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
        "Actual: the memory {0x01, 0x02, 0x03, 0x04, 0x05}");

    pfstest_protect_call(assert_longer_memory, &message_spy);

    assert_that("longer memory fails",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

static void assert_shorter_memory(void)
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
        "Actual: the memory {0x01, 0x02, 0x03, 0x04}");

    pfstest_protect_call(assert_shorter_memory, &message_spy);

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

static void assert_different_data_types(void)
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
        "Actual: the memory {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}"
        );

    pfstest_protect_call(assert_different_data_types, &message_spy);

    assert_that("different data types fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}
