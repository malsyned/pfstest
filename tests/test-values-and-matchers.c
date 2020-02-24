#include <limits.h>
#include <string.h>

#include "pfstest.h"
#include "capture-output.h"

setup()
{
    capture_output_init();
}

test(matches_the_pg_string_should_print_expected_string)
{
    const pfstest_pg_ptr char *s = pfstest_pg_str("foobar");
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the string \"foobar\"");
    pfstest_matcher_t *m = matches_the_pg_string(s);
    pfstest_matcher_print(m, message_spy);
    assert_that("matches_the_pg_string prints the expected string",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_short_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the short 32767");

    pfstest_value_print(the_short(32767), message_spy);

    assert_that("shorts print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_short_should_print_negative_shorts)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the short -32768");

    pfstest_value_print(the_short(-32768), message_spy);

    assert_that("negative shorts print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_shorts_should_match)
{
    assert_true("equal shorts match",
                pfstest_matcher_matches(equal_to(the_short(32767)),
                                        the_short(32767)));
}

test(unequal_shorts_should_not_match)
{
    assert_false("unequal shorts don't match",
                 pfstest_matcher_matches(equal_to(the_short(32767)),
                                         the_short(-32768)));
}

test(the_ushort_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the ushort 65535");

    pfstest_value_print(the_ushort(65535), message_spy);

    assert_that("ushorts print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(should_print_hex_ushorts)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the ushort 0xab2");

    pfstest_value_print(as_hex(the_ushort(0xab2)), message_spy);

    assert_that("ushorts print themselves as hex",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(equal_ushorts_should_match)
{
    assert_true("equal ushorts match",
                pfstest_matcher_matches(equal_to(the_ushort(65535)),
                                        the_ushort(65535)));
}

test(unequal_ushorts_should_not_match)
{
    assert_false("unequal ushorts don't match",
                 pfstest_matcher_matches(equal_to(the_ushort(65534)),
                                         the_ushort(65535)));
}

test(the_int_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the int 32767");

    pfstest_value_print(the_int(32767), message_spy);

    assert_that("ints print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_int_should_print_negative_ints)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the int -32768");

    pfstest_value_print(the_int(-32768), message_spy);

    assert_that("negative ints print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_ints_should_match)
{
    assert_true("equal ints match",
                pfstest_matcher_matches(equal_to(the_int(32767)),
                                        the_int(32767)));
}

test(unequal_ints_should_not_match)
{
    assert_false("unequal ints don't match",
                 pfstest_matcher_matches(equal_to(the_int(32767)),
                                         the_int(-32768)));
}

test(the_uint_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the uint 65535");

    pfstest_value_print(the_uint(65535), message_spy);

    assert_that("uints print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(should_print_hex_uints)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the uint 0xab2");

    pfstest_value_print(as_hex(the_uint(0xab2)), message_spy);

    assert_that("uints print themselves as hex",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(equal_uints_should_match)
{
    assert_true("equal uints match",
                pfstest_matcher_matches(equal_to(the_uint(65535)),
                                        the_uint(65535)));
}

test(unequal_uints_should_not_match)
{
    assert_false("unequal uints don't match",
                 pfstest_matcher_matches(equal_to(the_uint(65534)),
                                         the_uint(65535)));
}

test(the_long_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the long 2147483647");

    pfstest_value_print(the_long(2147483647l), message_spy);

    assert_that("longs print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_long_should_print_negative_longs)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the long -2147483648");

    pfstest_value_print(the_long((-2147483647l - 1)), message_spy);

    assert_that("negative longs print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}


test(equal_longs_should_match)
{
    assert_true("equal longs match",
                pfstest_matcher_matches(equal_to(the_long(2147483647l)),
                                        the_long(2147483647l)));
}

test(unequal_longs_should_not_match)
{
    assert_false("unequal longs don't match",
                 pfstest_matcher_matches(equal_to(the_long((-2147483647l - 1))),
                                         the_long(2147483647l)));
}

test(the_ulong_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the ulong 4294967295");

    pfstest_value_print(the_ulong(4294967295ul), message_spy);

    assert_that("ulongs print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(should_print_hex_ulongs)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the ulong 0xa1b2c3d");

    pfstest_value_print(as_hex(the_ulong(0xa1b2c3d)), message_spy);

    assert_that("ulongs print themselves as hex",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(equal_ulongs_should_match)
{
    assert_true("equal ulongs match",
                pfstest_matcher_matches(equal_to(the_ulong(4294967295ul)),
                                        the_ulong(4294967295ul)));
}

test(unequal_ulongs_should_not_match)
{
    assert_false("unequal ulongs don't match",
                 pfstest_matcher_matches(equal_to(the_ulong(4294967294ul)),
                                         the_ulong(4294967295ul)));
}

test(the_size_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the size_t 65535");

    pfstest_value_print(the_size(65535u), message_spy);

    assert_that("sizes print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(should_print_hex_sizes)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the size_t 0xab2");

    pfstest_value_print(as_hex(the_size(0xab2)), message_spy);

    assert_that("sizes print themselves as hex",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(equal_sizes_should_match)
{
    assert_true("equal sizes match",
                pfstest_matcher_matches(equal_to(the_size(65535u)),
                                        the_size(65535u)));
}

test(unequal_sizes_should_not_match)
{
    assert_false("unequal sizes don't match",
                 pfstest_matcher_matches(equal_to(the_size(65535u)),
                                         the_size(65534u)));
}

enum some_enum {
    e_foo,
    e_bar,
    e_baz
};

const pfstest_pg char e_foo_name[] = "e_foo";
const pfstest_pg char e_bar_name[] = "e_bar";
const pfstest_pg char e_baz_name[] = "e_baz";

const pfstest_pg_ptr char *const pfstest_pg some_enum_map[] = {
    e_foo_name,
    e_bar_name,
    e_baz_name,
    NULL,
};

test(the_enum_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the enum e_foo\nthe enum e_baz");

    pfstest_value_print(the_enum(e_foo, some_enum_map), message_spy);
    pfstest_reporter_print_char(message_spy, '\n');
    pfstest_value_print(the_enum(e_baz, some_enum_map), message_spy);

    assert_that("enums print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_enum_should_fall_back_to_printing_numeric_value)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the enum 3 [out of range]\n"
                       "the enum -1 [out of range]");

    pfstest_value_print(the_enum(3, some_enum_map), message_spy);
    pfstest_reporter_print_char(message_spy, '\n');
    pfstest_value_print(the_enum(-1, some_enum_map), message_spy);

    assert_that("enums print out-of-range values as numbers",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_enums_should_match)
{
    assert_true("equal enums match",
                pfstest_matcher_matches(
                    equal_to(the_enum(e_foo, some_enum_map)),
                    the_enum(e_foo, some_enum_map)));
}

test(unequal_enums_should_not_match)
{
    assert_false("unequal enums don't match",
                 pfstest_matcher_matches(
                     equal_to(the_enum(e_foo, some_enum_map)),
                     the_enum(e_baz, some_enum_map)));
}

test(the_bool_should_print_true)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the bool <true>");

    pfstest_value_print(the_bool(pfstest_true), message_spy);

    assert_that("bools print true",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_bool_should_print_false)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the bool <false>");

    pfstest_value_print(the_bool(pfstest_false), message_spy);

    assert_that("bools print false",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_bools_should_match)
{
    assert_true("equal bools match",
                pfstest_matcher_matches(equal_to(the_bool(pfstest_true)),
                                        the_bool(pfstest_true)));
}

test(unequal_bools_should_not_match)
{
    assert_false("unequal bools don't match",
                 pfstest_matcher_matches(equal_to(the_bool(pfstest_false)),
                                         the_bool(pfstest_true)));
}

test(different_but_equivalent_bools_should_match)
{
    assert_true("equivalent bools match",
                pfstest_matcher_matches(equal_to(the_bool(72)),
                                        the_bool(4)));
}

test(the_char_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the char 'c'");

    pfstest_value_print(the_char('c'), message_spy);

    assert_that("chars print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_char_should_escape_special_chars)
{    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the char '\\n'");

    pfstest_value_print(the_char('\n'), message_spy);

    assert_that("special chars should print escaped",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_chars_should_match)
{
    assert_true("equal chars match",
                pfstest_matcher_matches(equal_to(the_char('c')),
                                        the_char('c')));
}

test(unequal_chars_should_not_match)
{
    assert_false("unequal chars don't match",
                 pfstest_matcher_matches(equal_to(the_char('d')),
                                         the_char('c')));
}

test(the_string_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the string \"foo\"");
    char s1[] = "foo";

    pfstest_value_print(the_string(s1), message_spy);

    assert_that("strings print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_string_should_escape_special_characters)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the string \"\\t\\v\\\\\\\"\\?\\a\\b\\f\\n\\r\"");
    char s1[] = "\t\v\\\"\?\a\b\f\n\r";

    pfstest_value_print(the_string(s1), message_spy);

    assert_that("special chars should print escaped in strings",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_strings_should_match)
{
    char s1[] = "foo";
    char s2[] = "foo";

    assert_true("equal strings match",
                pfstest_matcher_matches(equal_to(the_string(s2)),
                                        the_string(s1)));
}

test(unequal_strings_should_not_match)
{
    char s1[] = "foo";
    char s2[] = "bar";

    assert_false("unequal strings don't match",
                 pfstest_matcher_matches(equal_to(the_string(s2)),
                                         the_string(s1)));
}

test(shorter_actual_string_should_not_match)
{
    char s1[] = "foo";
    char s2[] = "foobar";

    assert_false("shorter actual string doesn't match",
                 pfstest_matcher_matches(equal_to(the_string(s2)),
                                         the_string(s1)));
}

test(shorter_expected_string_should_not_match)
{
    char s1[] = "foobar";
    char s2[] = "foo";

    assert_false("shorter expected string doesn't match",
                 pfstest_matcher_matches(equal_to(the_string(s2)),
                                         the_string(s1)));
}

#ifdef __18CXX
/* All of the strings in this file overran 256 bytes of idata */
#pragma idata values_and_matchers_2
#endif

test(the_pointer_should_print_itself)
{
    /* Use only 16-bit pointers, since these should work on any
     * conceivable platform */
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the pointer <0xbeef>");

    pfstest_value_print(the_pointer((void *)0xbeef), message_spy);

    assert_that("pointers print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_pointer_should_print_null_specially)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the pointer <NULL>");

    pfstest_value_print(the_pointer(NULL), message_spy);

    assert_that("pointers print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_pointers_should_match)
{
    assert_true("equal pointers match",
                pfstest_matcher_matches(equal_to(the_pointer((void *)0xbeef)),
                                        the_pointer((void *)0xbeef)));
}

test(unequal_pointers_should_not_match)
{
    assert_false("equal pointers match",
                 pfstest_matcher_matches(equal_to(the_pointer((void *)0xbeef)),
                                         the_pointer((void *)0xdead)));
}

test(the_memory_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the memory {0x01, 0x02, 0x03, 0x04, 0x05}");
    char actual[] = {1, 2, 3, 4, 5};

    pfstest_value_print(the_memory(actual, sizeof(actual)), message_spy);

    assert_that("memory buffers print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_memory_should_match)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_true("equal memory buffers match",
                pfstest_matcher_matches(
                    equal_to(the_memory(expected, sizeof(expected))),
                    the_memory(actual, sizeof(actual))));
}

test(unequal_memory_should_not_match)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 4, 4, 5};

    assert_false("unequal memory buffers don't match",
                 pfstest_matcher_matches(
                     equal_to(the_memory(expected, sizeof(expected))),
                     the_memory(actual, sizeof(actual))));
}

test(shorter_actual_memory_should_not_match)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_false("shorter actual memory buffer doesn't match",
                 pfstest_matcher_matches(
                     equal_to(the_memory(expected, sizeof(expected) - 1)),
                     the_memory(actual, sizeof(actual))));
}

test(shorter_expected_memory_should_not_match)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_false("shorter expected memory buffer doesn't match",
                 pfstest_matcher_matches(
                     equal_to(the_memory(expected, sizeof(expected))),
                     the_memory(actual, sizeof(actual) - 1)));
}

test(anything_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("anything");

    pfstest_matcher_print(anything(), message_spy);

    assert_that("anything()'s printer prints \"anything\"",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(should_match_anything)
{
    assert_true("anything matches a short",
                pfstest_matcher_matches(anything(), the_short(5)));
    assert_true("anything matches an int",
                pfstest_matcher_matches(anything(), the_int(8)));
    assert_true("anything matches a pointer",
                pfstest_matcher_matches(anything(), the_pointer(NULL)));
}

test(should_match_memory_with_pointer)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_true("equal_to compares the_memory values to the_pointer values",
                pfstest_matcher_matches(
                    equal_to(the_memory(expected, sizeof(expected))),
                    the_pointer(actual)));
}

test(should_match_primitive_with_pointer)
{
    int actual = 5;
    int expected = 5;

    assert_that("equal_to compares expected value to the_pointer values",
                the_pointer(&actual), equal_to(the_int(expected)));
}

test(should_detect_data_type_mismatches)
{
    unsigned char buf[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    
    /* This test should be valid regardless of endianness, since all
     * slices of buf should look like USHRT_MAX, so the actual
     * contents of the data buffer should always match, requiring a
     * check of the sizes. This test may fail on a platform where
     * shorts are 64-bit, but I'm not aware of any such platform. */
    assert_false("different data types fail",
                 pfstest_matcher_matches(equal_to(the_ushort(USHRT_MAX)),
                                         the_memory(buf, sizeof(buf))));
}

test(equal_to_should_print_value)
{    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the int 3\nthe char 'c'");

    pfstest_matcher_print(equal_to(the_int(3)), message_spy);
    pfstest_reporter_print_char(message_spy, '\n');
    pfstest_matcher_print(equal_to(the_char('c')), message_spy);

    assert_that("int arrays print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(is_should_wrap_matchers)
{
    pfstest_matcher_t *m = equal_to(the_int(5));

    assert_that("is() wraps matchers without altering them",
                the_pointer(is(m)), equal_to(the_pointer(m)));
}

test(is_should_match_values_with_equal_to)
{
    assert_true("is(value) matches return true",
                pfstest_matcher_matches(is(the_int(5)), the_int(5)));
    assert_false("is(value) match failures return false",
                 pfstest_matcher_matches(is(the_int(5)), the_int(6)));
}

pfstest_case(calls_is_wrong)
{
    unsigned char *m = test_alloc(16);

    memset(m, 0xaa, 16);

    is(m);
}

test(is_should_reject_wrongly_tagged_arguments)
{
    /* This can't be a perfect test, since a memory block could start
     * with a tag by accident. It's worth trying, though. */

    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("is() called with something other than a matcher or "
                       "value");
    capture_test_results(calls_is_wrong);
    assert_that("is(<not a value or matcher>) fails the test",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(the_int_array_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("{ the int 1, the int 3, the int 32767, the int -4 }");
    int actual[] = {1, 3, 32767, -4};

    pfstest_value_print(
        the_int_array(actual, sizeof(actual)/sizeof(actual[0])), message_spy);

    assert_that("the_int_array prints itself",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(int_members_match_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("{ the int 1, the int 3, the int 32767, the int -5 }");

    pfstest_matcher_print(int_members_match(equal_to(the_int(1)),
                                            equal_to(the_int(3)),
                                            equal_to(the_int(32767)),
                                            equal_to(the_int(-5)),
                                            NULL),
                          message_spy);

    assert_that("int_members_match prints its member matchers",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(int_members_match_should_pass_matching_int_array)
{
    int actual[] = {1, 3, INT_MAX, -5};

    assert_true("Matcher arrays match against elements of integer array",
                pfstest_matcher_matches(
                    int_members_match(equal_to(the_int(1)),
                                      equal_to(the_int(3)),
                                      equal_to(the_int(INT_MAX)),
                                      equal_to(the_int(-5)),
                                      NULL),
                    the_int_array(actual, sizeof(actual)/sizeof(actual[0]))));
}

test(int_members_match_should_fail_nonmatching_array)
{
    int actual[] = {1, 3, 32767, -4};

    assert_false("Non-matching integer arrays fail",
                 pfstest_matcher_matches(
                     int_members_match(equal_to(the_int(1)),
                                       equal_to(the_int(3)),
                                       equal_to(the_int(INT_MAX)),
                                       equal_to(the_int(-5)),
                                       NULL),
                     the_int_array(actual, sizeof(actual)/sizeof(actual[0]))));
}

#ifdef PFSTEST_HAS_STDINT

test(the_u8_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the uint8_t 255");

    pfstest_value_print(the_u8(255), message_spy);

    assert_that("u8s print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(should_print_hex_u8s)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the uint8_t 0x0d");

    pfstest_value_print(as_hex(the_u8(0xd)), message_spy);

    assert_that("u8s print themselves as hex",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(equal_u8s_should_match)
{
    assert_true("equal u8s match",
                pfstest_matcher_matches(equal_to(the_u8(255)), the_u8(255)));
}

test(unequal_u8s_should_not_match)
{
    assert_false("equal u8s match",
                 pfstest_matcher_matches(equal_to(the_u8(255)), the_u8(254)));
}


test(the_u16_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the uint16_t 65535");

    pfstest_value_print(the_u16(65535), message_spy);

    assert_that("u16s print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(should_print_hex_u16s)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the uint16_t 0x0a1d");

    pfstest_value_print(as_hex(the_u16(0xa1d)), message_spy);

    assert_that("u16s print themselves as hex",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_u16s_should_match)
{
    assert_true("equal u16s match",
                pfstest_matcher_matches(equal_to(the_u16(65535)), the_u16(65535)));
}

test(unequal_u16s_should_not_match)
{
    assert_false("equal u16s match",
                 pfstest_matcher_matches(equal_to(the_u16(65535)), the_u16(65534)));
}

test(the_u32_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the uint32_t 4294967295");

    pfstest_value_print(the_u32(UINT32_MAX), message_spy);

    assert_that("u32s print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(should_print_hex_u32s)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the uint32_t 0x000a1db2");

    pfstest_value_print(as_hex(the_u32(0x000a1db2)), message_spy);

    assert_that("u32s print themselves as hex",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_u32s_should_match)
{
    assert_true("equal u32s match",
                pfstest_matcher_matches(equal_to(the_u32(UINT32_MAX)),
                                        the_u32(UINT32_MAX)));
}

test(unequal_u32s_should_not_match)
{
    assert_false("equal u32s match",
                 pfstest_matcher_matches(equal_to(the_u32(UINT32_MAX)),
                                         the_u32(UINT32_MAX - 1)));
}

test(the_u64_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the uint64_t 18446744073709551615");

    pfstest_value_print(the_u64(UINT64_MAX), message_spy);

    assert_that("u64s print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

#ifdef __GNUC__
/* Work around a bug in glibc's stdint.h when -pedantic is used */
#define UINT64_C_NOWARN(x) (__extension__ UINT64_C(x))
#else
#define UINT64_C_NOWARN(x) UINT64_C(x)
#endif

test(should_print_hex_u64s)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the uint64_t 0x000fedcba9876543");

    pfstest_value_print(as_hex(the_u64(UINT64_C_NOWARN(0x000fedcba9876543))),
                        message_spy);

    assert_that("u64s print themselves as hex",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_u64s_should_match)
{
    assert_true("equal u64s match",
                pfstest_matcher_matches(equal_to(the_u64(UINT64_MAX)),
                                        the_u64(UINT64_MAX)));
}

test(unequal_u64s_should_not_match)
{
    assert_false("equal u64s match",
                 pfstest_matcher_matches(equal_to(the_u64(UINT64_MAX)),
                                         the_u64(UINT64_MAX - 1)));
}

#endif /* defined(PFSTEST_HAS_STDINT) */
