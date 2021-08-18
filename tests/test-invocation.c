/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest.h"
#include "pfstest-cli.h"

#include <string.h>

#include "capture-output.h"

static pfstest_arguments_t args;

setup()
{
    capture_output_init();
    memset(&args, 0xa5, sizeof(args));
}

test(should_parse_program_name_argument)
{
    char *argv[2];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    argv[0] = argv_0;
    argv[1] = NULL;
    
    pfstest_arguments_parse(&args, argc, argv);

    assert_that("Program name argument is stored in args struct",
                the_pointer(args.program_name), equal_to(the_pointer(argv[0])));
}

test(should_return_true_for_good_arg_lists)
{
    char *argv[2];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    argv[0] = argv_0;
    argv[1] = NULL;
    

    assert_true("Good arg lists return true",
                pfstest_arguments_parse(&args, argc, argv));
}

test(should_accept_0_length_arg_list)
{
    char *argv[] = {NULL};
    int argc = sizeof(argv)/sizeof(argv[0]);
    
    assert_true("Zero length argument lists are accepted",
                 pfstest_arguments_parse(&args, argc, argv));
}

test(should_set_program_name_NULL_on_bad_program_name_parse)
{
    char *argv[] = {NULL};
    int argc = sizeof(argv)/sizeof(argv[0]);
    
    pfstest_arguments_parse(&args, argc, argv);

    assert_null("Zero length argument lists set program_name to NULL",
                args.program_name);
}

test(should_set_default_args_from_simplest_arg_list)
{
    char *argv[2];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    argv[0] = argv_0;
    argv[1] = NULL;

    pfstest_arguments_parse(&args, argc, argv);

    assert_false("verbose flag defaults to false", args.verbose);
    assert_false("xml flag defaults to false", args.xml);
    assert_false("color flag defaults to false", args.color);
    assert_false("print_register_commands flag defaults to false",
                 args.print_register_commands);
    assert_null("filter_file defaults to false", args.filter_file);
    assert_null("filter_name defaults to false", args.filter_name);
}

test(should_parse_c_flag)
{
    char *argv[3];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    char argv_1[] = "-c";
    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = NULL;

    pfstest_arguments_parse(&args, argc, argv);

    assert_true("-c flag is parsed", args.color);
}

test(should_parse_v_flag)
{
    char *argv[3];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    char argv_1[] = "-v";
    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = NULL;
    
    pfstest_arguments_parse(&args, argc, argv);

    assert_true("-v flag is parsed", args.verbose);
}

test(should_parse_x_flag)
{
    char *argv[3];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    char argv_1[] = "-x";
    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = NULL;

    pfstest_arguments_parse(&args, argc, argv);

    assert_true("-x flag is parsed", args.xml);
}

test(x_flag_should_reset_v_flag)
{
    char *argv[4];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    char argv_1[] = "-v";
    char argv_2[] = "-x";
    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = argv_2;
    argv[3] = NULL;

    pfstest_arguments_parse(&args, argc, argv);

    assert_true("-x flag is parsed", args.xml);
    assert_false("-v flag has been reset", args.verbose);
}

test(x_flag_should_nullify_v_flag)
{
    char *argv[4];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    char argv_1[] = "-x";
    char argv_2[] = "-v";
    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = argv_2;
    argv[3] = NULL;

    pfstest_arguments_parse(&args, argc, argv);

    assert_true("-x flag is parsed", args.xml);
    assert_false("-v flag has been reset", args.verbose);
}

test(should_parse_r_flag)
{
    char *argv[3];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    char argv_1[] = "-r";
    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = NULL;
    
    pfstest_arguments_parse(&args, argc, argv);

    assert_true("-r flag is parsed", args.print_register_commands);
}

test(should_parse_f_flag)
{
    char *argv[4];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    char argv_1[] = "-f";
    char argv_2[] = "filter_file.c";
    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = argv_2;
    argv[3] = NULL;

    pfstest_arguments_parse(&args, argc, argv);

    assert_that("-f flag is parsed",
                the_pointer(args.filter_file), equal_to(the_pointer(argv[2])));
}

test(f_flag_should_require_argument)
{
    char *argv[3];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    char argv_1[] = "-f";
    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = NULL;

    pfstest_arguments_parse(&args, argc, argv);

    assert_false("-f flag requires an argument",
                 pfstest_arguments_parse(&args, argc, argv));
}

test(should_parse_n_flag)
{
    char *argv[4];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    char argv_1[] = "-n";
    char argv_2[] = "test_name";
    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = argv_2;
    argv[3] = NULL;

    pfstest_arguments_parse(&args, argc, argv);

    assert_that("-n flag is parsed",
                the_pointer(args.filter_name), equal_to(the_pointer(argv[2])));
    assert_true("-n flag implies verbose", args.verbose);
}

test(n_flag_should_require_argument)
{
    char *argv[3];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    char argv_1[] = "-n";
    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = NULL;

    pfstest_arguments_parse(&args, argc, argv);

    assert_false("-n flag requires an argument",
                 pfstest_arguments_parse(&args, argc, argv));
}

test(should_reject_unknown_arguments)
{
    char *argv[3];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    char argv_1[] = "-foobar";
    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = NULL;
    
    assert_false("Bad arg lists return false",
                 pfstest_arguments_parse(&args, argc, argv));
}

test(should_parse_multiple_arguments)
{
    char *argv[5];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    char argv_1[] = "-v";
    char argv_2[] = "-f";
    char argv_3[] = "test_name";
    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = argv_2;
    argv[3] = argv_3;
    argv[4] = NULL;

    pfstest_arguments_parse(&args, argc, argv);

    assert_true("-v flag is parsed", args.verbose);
    assert_that("-f flag is parsed",
                the_pointer(args.filter_file), equal_to(the_pointer(argv[3])));
}

pfstest_case(test1) {}
pfstest_case(test2) {}

pfstest_plugin_define(plugin1, NULL, NULL, NULL);
pfstest_plugin_define(plugin2, NULL, NULL, NULL);

test(should_print_register_commands)
{
    pfstest_list_t suite = PFSTEST_LIST_EMPTY();
    pfstest_list_t plugins = PFSTEST_LIST_EMPTY();

    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "    register_plugin(plugin1);\n"
        "    register_plugin(plugin2);\n"
        "    register_test(test1);\n"
        "    register_test(test2);\n");

    pfstest_suite_register_test(&suite, test1);
    pfstest_suite_register_test(&suite, test2);
    pfstest_plugin_list_register_plugin(&plugins, plugin1);
    pfstest_plugin_list_register_plugin(&plugins, plugin2);

    pfstest_print_register_commands(capture_output_char, &plugins, &suite);

    assert_that("Test registrations are printed",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(should_print_usage)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "usage: program_name [-r] [-v|-x] [-c] [-f source-file] [-n test-name]\n");
    
    char program_name[] = "program_name";
    pfstest_print_usage(capture_output_char, program_name);

    assert_that("Usage message is printed",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(print_usage_should_cope_with_null_program_name)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "usage: [-r] [-v|-x] [-c] [-f source-file] [-n test-name]\n");

    pfstest_print_usage(capture_output_char, NULL);

    assert_that("Usage message is printed without program name",
                the_string(captured_output),
                matches_the_pg_string(expected));
}

test(print_usage_should_cope_with_empty_program_name)
{
    const pfstest_pg_ptr char *expected = pfstest_pg_str(
        "usage: [-r] [-v|-x] [-c] [-f source-file] [-n test-name]\n");

    char program_name[] = "";
    pfstest_print_usage(capture_output_char, program_name);

    assert_that("Usage message is printed with empty program name",
                the_string(captured_output),
                matches_the_pg_string(expected));
}