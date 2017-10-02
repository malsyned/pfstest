#include <stdint.h>

#include "pfstest.h"
#include "pfstest-stdint.h"
#include "capture-output.h"

before_tests(set_up_stdint)
{
    capture_output_init();
}

test(should_match_u8)
{
    assert_that("same u8s pass", the_u8(5), is_the_u8(5));
}

pfstest_case(assert_different_u8s)
{
    assert_that("", the_u8(5), is_the_u8(6));
}

test(should_fail_on_different_u8s)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the uint8_t 6\n"
        "Actual:   the uint8_t 5");

    capture_test_results(assert_different_u8s);

    assert_that("different u8s fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_match_u16)
{
    assert_that("same u16s pass", the_u16(256), is_the_u16(256));
}

pfstest_case(assert_different_u16s)
{
    assert_that("", the_u16(256), is_the_u16(257));
}

test(should_fail_on_different_u16s)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "Failed assertion\n"
        "Expected: the uint16_t 257\n"
        "Actual:   the uint16_t 256");

    capture_test_results(assert_different_u16s);

    assert_that("different u16s fail",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

