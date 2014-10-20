#include <limits.h>

#include "pfstest.h"

test(should_match_shorts)
{
    assert_that("same shorts pass", the_short(5), is_the_short(5));
}

failing_test(should_fail_on_different_shorts)
{
    assert_that("different shorts fail", the_short(5), is_the_short(6));
}

test(should_match_ushorts)
{
    assert_that("same ushorts pass", the_ushort(5), is_the_ushort(5));
}

failing_test(should_fail_on_different_ushorts)
{
    assert_that("different ushorts fail", the_ushort(5), is_the_ushort(6));
}

test(should_match_ints)
{
    assert_that("same ints pass", the_int(5), is_the_int(5));
}

failing_test(should_fail_on_different_ints)
{
    assert_that("different ints fail", the_int(-6), is_the_int(6));
}

test(should_match_uints)
{
    assert_that("same uints pass",
                the_uint(5), is_the_uint(5));
}

failing_test(should_fail_on_different_uints)
{
    assert_that("different uints fail",
                the_uint(5), is_the_uint(6));
}

failing_test(should_detect_data_type_mismatches)
{
    unsigned char buf[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    
    /* This test should be valid regardless of endianness, since all
     * slices of buf should look like USHRT_MAX, so the actual
     * contents of the data buffer should always match, requiring a
     * check of the sizes. This test may fail on a platform where
     * shorts are 64-bit, but I'm not aware of any such platform. */
    assert_that("different data types fail",
                the_memory(buf, sizeof(buf)), is_the_ushort(USHRT_MAX));
}

test(should_match_chars)
{
    assert_that("same chars pass", the_char('c'), is_the_char('c'));
}

failing_test(should_fail_on_different_chars)
{
    assert_that("different chars fail", the_char('c'), is_the_char('d'));
}

test(should_match_strings)
{
    assert_that("same strings pass", the_string("foo"), is_the_string("foo"));
}

failing_test(should_fail_on_different_strings)
{
    assert_that("different strings fail",
                the_string("foo"), is_the_string("bar"));
}

failing_test(should_fail_on_shorter_actual_string)
{
    assert_that("shorter actual string fails",
                the_string("foo"), is_the_string("foobar"));
}

failing_test(should_fail_on_shorter_expected_string)
{
    assert_that("shorter expected string fails",
                the_string("foobar"), is_the_string("foo"));
}

test(should_match_pointers)
{
    char *p1 = "foo";
    char *p2 = p1;

    assert_that("same pointers pass",
                the_pointer(p1), is_the_pointer(p2));
}

failing_test(should_fail_on_different_pointers)
{
    char *p1 = "foo";
    char *p2 = "bar";

    assert_that("different pointers fail",
                the_pointer(p1), is_the_pointer(p2));
}

test(should_match_memory)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_that("same memory passes",
                the_memory(actual, sizeof(actual)),
                is_the_memory(expected, sizeof(expected)));
}

failing_test(should_fail_on_different_memory)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 4, 4, 5};

    assert_that("different memory fails",
                the_memory(actual, sizeof(actual)),
                is_the_memory(expected, sizeof(expected)));
}

failing_test(should_fail_expecting_shorter_memory)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_that("longer memory fails",
                the_memory(actual, sizeof(actual)),
                is_the_memory(expected, sizeof(expected) - 1));
}

failing_test(should_fail_expecting_longer_memory)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_that("shorter memory fails",
                the_memory(actual, sizeof(actual) - 1),
                is_the_memory(expected, sizeof(expected)));
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
