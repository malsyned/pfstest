#include "pfstest.h"

#include <string.h>

static pfstest_arguments_t args;

before_tests(set_up_interface_tests)
{
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
                the_pointer(args.program_name), is_the_pointer(argv[0]));
}

test(should_return_true_for_good_arg_lists)
{
    char *argv[2];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    argv[0] = argv_0;
    argv[1] = NULL;
    

    assert_that("Good arg lists return true",
                the_bool(pfstest_arguments_parse(&args, argc, argv)),
                is_the_bool(true));
}


test(should_fail_to_parse_0_length_arg_list)
{
    char *argv[] = {NULL};
    int argc = sizeof(argv)/sizeof(argv[0]);
    
    assert_that("Zero length argument lists are rejected",
                the_bool(pfstest_arguments_parse(&args, argc, argv)),
                is_the_bool(false));
}

test(should_set_program_name_NULL_on_bad_program_name_parse)
{
    char *argv[] = {NULL};
    int argc = sizeof(argv)/sizeof(argv[0]);
    
    pfstest_arguments_parse(&args, argc, argv);

    assert_that("Zero length argument lists set program_name to NULL",
                the_pointer(args.program_name),
                is_the_pointer(NULL));
}

test(should_set_default_args_from_simplest_arg_list)
{
    char *argv[2];
    int argc = sizeof(argv)/sizeof(argv[0]);
    char argv_0[] = "progname";
    argv[0] = argv_0;
    argv[1] = NULL;

    pfstest_arguments_parse(&args, argc, argv);

    assert_that("verbose flag defaults to false",
                the_bool(args.verbose), is_the_bool(false));
    assert_that("print_register_commands flag defaults to false",
                the_bool(args.print_register_commands), is_the_bool(false));
    assert_that("filter_file defaults to false",
                the_pointer(args.filter_file), is_the_pointer(NULL));
    assert_that("filter_name defaults to false",
                the_pointer(args.filter_name), is_the_pointer(NULL));
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

    assert_that("-v flag is parsed",
                the_bool(args.verbose), is_the_bool(true));
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

    assert_that("-r flag is parsed",
                the_bool(args.print_register_commands), is_the_bool(true));
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
                the_pointer(args.filter_file), is_the_pointer(argv[2]));
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

    assert_that("-f flag requires an argument",
                the_bool(pfstest_arguments_parse(&args, argc, argv)),
                is_the_bool(false));
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
                the_pointer(args.filter_name), is_the_pointer(argv[2]));
    assert_that("-n flag implies verbose",
                the_bool(args.verbose), is_the_bool(true));
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

    assert_that("-n flag requires an argument",
                the_bool(pfstest_arguments_parse(&args, argc, argv)),
                is_the_bool(false));
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
    
    assert_that("Bad arg lists return false",
                the_bool(pfstest_arguments_parse(&args, argc, argv)),
                is_the_bool(false));
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

    assert_that("-v flag is parsed",
                the_bool(args.verbose), is_the_bool(true));
    assert_that("-f flag is parsed",
                the_pointer(args.filter_file), is_the_pointer(argv[3]));
}