#include <limits.h>

#include "pfstest.h"
#include "capture-output.h"

before_tests(set_up_values_and_matchers)
{
    capture_output_init();
}

test(the_short_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the short 32767");

    pfstest_value_print(message_spy, the_short(32767));

    assert_that("shorts print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_short_should_print_negative_shorts)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the short -32768");

    pfstest_value_print(message_spy, the_short(-32768));

    assert_that("negative shorts print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_shorts_should_match)
{
    assert_true("equal shorts match",
                pfstest_matcher_matches(is(the_short(32767)),
                                        the_short(32767)));
}

test(unequal_shorts_should_not_match)
{
    assert_false("unequal shorts don't match",
                 pfstest_matcher_matches(is(the_short(32767)),
                                         the_short(-32768)));
}

test(the_ushort_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the ushort 65535");

    pfstest_value_print(message_spy, the_ushort(65535));

    assert_that("ushorts print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(should_print_hex_ushorts)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the ushort 0xab2");

    pfstest_value_print(message_spy, as_hex(the_ushort(0xab2)));

    assert_that("ushorts print themselves as hex",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(equal_ushorts_should_match)
{
    assert_true("equal ushorts match",
                pfstest_matcher_matches(is(the_ushort(65535)),
                                        the_ushort(65535)));
}

test(unequal_ushorts_should_not_match)
{
    assert_false("unequal ushorts don't match",
                 pfstest_matcher_matches(is(the_ushort(65534)),
                                         the_ushort(65535)));
}

test(the_int_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the int 32767");

    pfstest_value_print(message_spy, the_int(32767));

    assert_that("ints print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_int_should_print_negative_ints)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the int -32768");

    pfstest_value_print(message_spy, the_int(-32768));

    assert_that("negative ints print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_ints_should_match)
{
    assert_true("equal ints match",
                pfstest_matcher_matches(is(the_int(32767)),
                                        the_int(32767)));
}

test(unequal_ints_should_not_match)
{
    assert_false("unequal ints don't match",
                 pfstest_matcher_matches(is(the_int(32767)),
                                         the_int(-32768)));
}

test(the_uint_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the uint 65535");

    pfstest_value_print(message_spy, the_uint(65535));

    assert_that("uints print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(should_print_hex_uints)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the uint 0xab2");

    pfstest_value_print(message_spy, as_hex(the_uint(0xab2)));

    assert_that("uints print themselves as hex",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(equal_uints_should_match)
{
    assert_true("equal uints match",
                pfstest_matcher_matches(is(the_uint(65535)),
                                        the_uint(65535)));
}

test(unequal_uints_should_not_match)
{
    assert_false("unequal uints don't match",
                 pfstest_matcher_matches(is(the_uint(65534)),
                                         the_uint(65535)));
}

test(the_long_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the long 2147483647");

    pfstest_value_print(message_spy, the_long(2147483647l));

    assert_that("longs print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_long_should_print_negative_longs)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the long -2147483648");

    pfstest_value_print(message_spy, the_long((-2147483647l - 1)));

    assert_that("negative longs print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}


test(equal_longs_should_match)
{
    assert_true("equal longs match",
                pfstest_matcher_matches(is(the_long(2147483647l)),
                                        the_long(2147483647l)));
}

test(unequal_longs_should_not_match)
{
    assert_false("unequal longs don't match",
                 pfstest_matcher_matches(is(the_long((-2147483647l - 1))),
                                         the_long(2147483647l)));
}

test(the_ulong_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the ulong 4294967295");

    pfstest_value_print(message_spy, the_ulong(4294967295ul));

    assert_that("ulongs print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(should_print_hex_ulongs)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the ulong 0xa1b2c3d");

    pfstest_value_print(message_spy, as_hex(the_ulong(0xa1b2c3d)));

    assert_that("ulongs print themselves as hex",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(equal_ulongs_should_match)
{
    assert_true("equal ulongs match",
                pfstest_matcher_matches(is(the_ulong(4294967295ul)),
                                        the_ulong(4294967295ul)));
}

test(unequal_ulongs_should_not_match)
{
    assert_false("unequal ulongs don't match",
                 pfstest_matcher_matches(is(the_ulong(4294967294ul)),
                                         the_ulong(4294967295ul)));
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

    pfstest_value_print(message_spy, the_enum(e_foo, some_enum_map));
    pfstest_reporter_print_char(message_spy, '\n');
    pfstest_value_print(message_spy, the_enum(e_baz, some_enum_map));

    assert_that("enums print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_enum_should_fall_back_to_printing_numeric_value)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the enum 3 [out of range]\n"
                       "the enum -1 [out of range]");

    pfstest_value_print(message_spy, the_enum(3, some_enum_map));
    pfstest_reporter_print_char(message_spy, '\n');
    pfstest_value_print(message_spy, the_enum(-1, some_enum_map));

    assert_that("enums print out-of-range values as numbers",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_enums_should_match)
{
    assert_true("equal enums match",
                pfstest_matcher_matches(is(the_enum(e_foo, some_enum_map)),
                                        the_enum(e_foo, some_enum_map)));
}

test(unequal_enums_should_not_match)
{
    assert_false("unequal enums don't match",
                 pfstest_matcher_matches(is(the_enum(e_foo, some_enum_map)),
                                         the_enum(e_baz, some_enum_map)));
}

test(the_bool_should_print_true)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the bool <true>");

    pfstest_value_print(message_spy, the_bool(pfstest_true));

    assert_that("bools print true",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_bool_should_print_false)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the bool <false>");

    pfstest_value_print(message_spy, the_bool(pfstest_false));

    assert_that("bools print false",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_bools_should_match)
{
    assert_true("equal bools match",
                pfstest_matcher_matches(is(the_bool(pfstest_true)),
                                        the_bool(pfstest_true)));
}

test(unequal_bools_should_not_match)
{
    assert_false("unequal bools don't match",
                 pfstest_matcher_matches(is(the_bool(pfstest_false)),
                                         the_bool(pfstest_true)));
}

test(different_but_equivalent_bools_should_match)
{
    assert_true("equivalent bools match",
                pfstest_matcher_matches(is(the_bool(72)),
                                        the_bool(4)));
}

test(the_char_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the char 'c'");

    pfstest_value_print(message_spy, the_char('c'));

    assert_that("chars print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_char_should_escape_special_chars)
{    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the char '\\n'");

    pfstest_value_print(message_spy, the_char('\n'));

    assert_that("special chars should print escaped",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_chars_should_match)
{
    assert_true("equal chars match",
                pfstest_matcher_matches(is(the_char('c')),
                                        the_char('c')));
}

test(unequal_chars_should_not_match)
{
    assert_false("unequal chars don't match",
                pfstest_matcher_matches(is(the_char('d')),
                                        the_char('c')));
}

test(the_string_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the string \"foo\"");
    char s1[] = "foo";

    pfstest_value_print(message_spy, the_string(s1));

    assert_that("strings print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_string_should_escape_special_characters)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the string \"\\t\\v\\\\\\\"\\?\\a\\b\\f\\n\\r\"");
    char s1[] = "\t\v\\\"\?\a\b\f\n\r";

    pfstest_value_print(message_spy, the_string(s1));

    assert_that("special chars should print escaped in strings",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_strings_should_match)
{
    char s1[] = "foo";
    char s2[] = "foo";

    assert_true("equal strings match",
                pfstest_matcher_matches(is(the_string(s2)),
                                        the_string(s1)));
}

test(unequal_strings_should_not_match)
{
    char s1[] = "foo";
    char s2[] = "bar";

    assert_false("unequal strings don't match",
                 pfstest_matcher_matches(is(the_string(s2)),
                                         the_string(s1)));
}

test(shorter_actual_string_should_not_match)
{
    char s1[] = "foo";
    char s2[] = "foobar";

    assert_false("shorter actual string doesn't match",
                 pfstest_matcher_matches(is(the_string(s2)),
                                         the_string(s1)));
}

test(shorter_expected_string_should_not_match)
{
    char s1[] = "foobar";
    char s2[] = "foo";

    assert_false("shorter expected string doesn't match",
                 pfstest_matcher_matches(is(the_string(s2)),
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

    pfstest_value_print(message_spy, the_pointer((void *)0xbeef));

    assert_that("pointers print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_pointer_should_print_null_specially)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the pointer <NULL>");

    pfstest_value_print(message_spy, the_pointer(NULL));

    assert_that("pointers print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_pointers_should_match)
{
    assert_true("equal pointers match",
                pfstest_matcher_matches(is(the_pointer((void *)0xbeef)),
                                        the_pointer((void *)0xbeef)));
}

test(unequal_pointers_should_not_match)
{
    assert_false("equal pointers match",
                 pfstest_matcher_matches(is(the_pointer((void *)0xbeef)),
                                         the_pointer((void *)0xdead)));
}

test(the_memory_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the memory {0x01, 0x02, 0x03, 0x04, 0x05}");
    char actual[] = {1, 2, 3, 4, 5};

    pfstest_value_print(message_spy, the_memory(actual, sizeof(actual)));

    assert_that("memory buffers print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_memory_should_match)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_true("equal memory buffers match",
                pfstest_matcher_matches(
                    is(the_memory(expected, sizeof(expected))),
                    the_memory(actual, sizeof(actual))));
}

test(unequal_memory_should_not_match)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 4, 4, 5};

    assert_false("unequal memory buffers don't match",
                 pfstest_matcher_matches(
                     is(the_memory(expected, sizeof(expected))),
                     the_memory(actual, sizeof(actual))));
}

test(shorter_actual_memory_should_not_match)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_false("shorter actual memory buffer doesn't match",
                 pfstest_matcher_matches(
                     is(the_memory(expected, sizeof(expected) - 1)),
                     the_memory(actual, sizeof(actual))));
}

test(shorter_expected_memory_should_not_match)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_false("shorter expected memory buffer doesn't match",
                 pfstest_matcher_matches(
                     is(the_memory(expected, sizeof(expected))),
                     the_memory(actual, sizeof(actual) - 1)));
}

test(is_anything_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("anything");

    pfstest_matcher_print(message_spy, is_anything);

    assert_that("is_anything()'s printer prints \"anything\"",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(should_match_anything)
{
    assert_true("is_anything matches a short",
                pfstest_matcher_matches(is_anything, the_short(5)));
    assert_true("is_anything matches an int",
                pfstest_matcher_matches(is_anything, the_int(8)));
    assert_true("is_anything matches a pointer",
                pfstest_matcher_matches(is_anything, the_pointer(NULL)));
}

test(should_match_memory_with_pointer)
{
    char actual[] = {1, 2, 3, 4, 5};
    char expected[] = {1, 2, 3, 4, 5};

    assert_true("is_the_memory matches against the_pointer values",
                pfstest_matcher_matches(
                    is(the_memory(expected, sizeof(expected))),
                    the_pointer(actual)));
}

test(should_match_primitive_with_pointer)
{
    int actual = 5;
    int expected = 5;

    assert_that("is_the_int matches against the_pointer values",
                the_pointer(&actual), is(the_int(expected)));
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
                pfstest_matcher_matches(is(the_ushort(USHRT_MAX)),
                                        the_memory(buf, sizeof(buf))));
}

test(is_should_print_value)
{    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the int 3\nthe char 'c'");

    pfstest_matcher_print(message_spy, is(the_int(3)));
    pfstest_reporter_print_char(message_spy, '\n');
    pfstest_matcher_print(message_spy, is(the_char('c')));

    assert_that("int arrays print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(the_int_array_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("{ the int 1, the int 3, the int 32767, the int -4 }");
    int actual[] = {1, 3, 32767, -4};

    pfstest_value_print(
        message_spy, the_int_array(actual, sizeof(actual)/sizeof(actual[0])));

    assert_that("the_int_array prints itself",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(int_members_match_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("{ the int 1, the int 3, the int 32767, the int -5 }");

    pfstest_matcher_print(message_spy, int_members_match(is(the_int(1)),
                                                         is(the_int(3)),
                                                         is(the_int(32767)),
                                                         is(the_int(-5)),
                                                         NULL));

    assert_that("int_members_match prints its member matchers",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(int_members_match_should_pass_matching_int_array)
{
    int actual[] = {1, 3, INT_MAX, -5};

    assert_true("Matcher arrays match against elements of integer array",
                pfstest_matcher_matches(
                    int_members_match(is(the_int(1)),
                                      is(the_int(3)),
                                      is(the_int(INT_MAX)),
                                      is(the_int(-5)),
                                      NULL),
                    the_int_array(actual, sizeof(actual)/sizeof(actual[0]))));
}

test(int_members_match_should_fail_nonmatching_array)
{
    int actual[] = {1, 3, 32767, -4};

    assert_false("Non-matching integer arrays fail",
                pfstest_matcher_matches(
                    int_members_match(is(the_int(1)),
                                      is(the_int(3)),
                                      is(the_int(INT_MAX)),
                                      is(the_int(-5)),
                                      NULL),
                    the_int_array(actual, sizeof(actual)/sizeof(actual[0]))));
}

#ifdef PFSTEST_HAS_STDINT

test(the_u8_should_print_itself)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str("the uint8_t 255");

    pfstest_value_print(message_spy, the_u8(255));

    assert_that("u8s print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(should_print_hex_u8s)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the uint8_t 0x0d");

    pfstest_value_print(message_spy, as_hex(the_u8(0xd)));

    assert_that("u8s print themselves as hex",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(equal_u8s_should_match)
{
    assert_true("equal u8s match",
                pfstest_matcher_matches(is(the_u8(255)), the_u8(255)));
}

test(unequal_u8s_should_not_match)
{
    assert_false("equal u8s match",
                pfstest_matcher_matches(is(the_u8(255)), the_u8(254)));
}


test(the_u16_should_print_itself)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the uint16_t 65535");

    pfstest_value_print(message_spy, the_u16(65535));

    assert_that("u16s print themselves",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(should_print_hex_u16s)
{
    const pfstest_pg_ptr char *expected =
        pfstest_pg_str("the uint16_t 0x0a1d");

    pfstest_value_print(message_spy, as_hex(the_u16(0xa1d)));

    assert_that("u16s print themselves as hex",
                the_string(captured_output), matches_the_pg_string(expected));
}

test(equal_u16s_should_match)
{
    assert_true("equal u16s match",
                pfstest_matcher_matches(is(the_u16(65535)), the_u16(65535)));
}

test(unequal_u16s_should_not_match)
{
    assert_false("equal u16s match",
                pfstest_matcher_matches(is(the_u16(65535)), the_u16(65534)));
}

#endif /* defined(PFSTEST_HAS_STDINT) */
