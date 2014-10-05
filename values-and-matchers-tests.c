#include <stdint.h>

#include "pfstest.h"

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
                the_uint(UINTMAX_MAX), is_the_uint(UINTMAX_MAX));
}

failing_test(should_fail_on_different_uints)
{
    assert_that("different uints fail",
                the_uint(5), is_the_uint(UINTMAX_MAX));
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
