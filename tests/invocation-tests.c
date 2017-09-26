#include "pfstest.h"

#include <string.h>

#include "capture-output.h"

static pfstest_arguments_t args;

before_tests(set_up_interface_tests)
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
    assert_that("xml flag defaults to false",
                the_bool(args.xml), is_the_bool(false));
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

    assert_that("-x flag is parsed",
                the_bool(args.xml), is_the_bool(true));
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

    assert_that("-x flag is parsed",
                the_bool(args.xml), is_the_bool(true));
    assert_that("-v flag has ben reset",
                the_bool(args.verbose), is_the_bool(false));
}

test(v_flag_should_reset_x_flag)
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

    assert_that("-v flag is parsed",
                the_bool(args.verbose), is_the_bool(true));
    assert_that("-x flag has ben reset",
                the_bool(args.xml), is_the_bool(false));
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

pfstest_case(test1) {}
pfstest_case(test2) {}
pfstest_hook(before1) {}
pfstest_hook(before2) {}
pfstest_hook(after1) {}
pfstest_hook(after2) {}

test(should_print_register_commands)
{
    pfstest_list_t suite = PFSTEST_LIST_EMPTY();
    pfstest_list_t before_hooks = PFSTEST_LIST_EMPTY();
    pfstest_list_t after_hooks = PFSTEST_LIST_EMPTY();

    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "    register_before(before1);\n"
        "    register_before(before2);\n"
        "    register_after(after1);\n"
        "    register_after(after2);\n"
        "    register_test(test1);\n"
        "    register_test(test2);\n");

    pfstest_suite_register_test(&suite, test1);
    pfstest_suite_register_test(&suite, test2);
    pfstest_hook_list_register_hook(&before_hooks, before1);
    pfstest_hook_list_register_hook(&before_hooks, before2);
    pfstest_hook_list_register_hook(&after_hooks, after1);
    pfstest_hook_list_register_hook(&after_hooks, after2);

    pfstest_print_register_commands(capture_output_char,
                                    &before_hooks, &after_hooks, &suite);

    assert_that("Test registrations are printed",
                the_string(captured_output),
                matches_the_nv_string(expected));
}

test(should_print_usage)
{
    const pfstest_nv_ptr char *expected = pfstest_nv_string(
        "usage: program_name [-r] [-v] [-f source-file] [-n test-name]\n");
    
    char program_name[] = "program_name";
    pfstest_print_usage(capture_output_char, program_name);

    assert_that("Usage message is printed",
                the_string(captured_output),
                matches_the_nv_string(expected));
}
