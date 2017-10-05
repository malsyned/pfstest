#include "pfstest-cli.h"

#include <stdio.h>

#include "pfstest-core.h"
#include "pfstest-reporters-standard.h"
#include "pfstest-reporter-xml.h"
#include "pfstest-alloc.h"

static void print_nv_string(int (*print_char)(int),
                            const pfstest_nv_ptr char *s)
{
    char c;

    while (pfstest_memcpy_nv(&c, s, sizeof(c)), c) {
        print_char(c);
        s++;
    }
}

static void print_string(int (*print_char)(int), const char *s)
{
    while (*s) {
        print_char(*s++);
    }
}

static int stdout_print_char(int c)
{
    int r = putc(c, stdout);
    fflush(stdout);
    return r;
}

static void print_register_hook_commands(
    int (*print_char)(int), pfstest_list_t *list,
    const pfstest_nv_ptr char *list_name)
{
    pfstest_list_node_t *hook_node;
    _pfstest_hook_nv_t nv_data;

    pfstest_list_iter (hook_node, list) {
        pfstest_hook_t *hook = (pfstest_hook_t *)hook_node;
        pfstest_memcpy_nv(&nv_data, hook->nv_data, sizeof(nv_data));

        print_nv_string(print_char, pfstest_nv_string("    register_"));
        print_nv_string(print_char, list_name);
        print_nv_string(print_char, pfstest_nv_string("("));
        print_nv_string(print_char, nv_data.name);
        print_nv_string(print_char, pfstest_nv_string(");\n"));
    }
}

static void print_register_test_commands(int (*print_char)(int),
                                         pfstest_list_t *tests)
{
    pfstest_list_node_t *test_node;
    _pfstest_test_nv_t nv_data;

    pfstest_list_iter (test_node, tests) {
        pfstest_t *test = (pfstest_t *)test_node;
        pfstest_memcpy_nv(&nv_data, test->nv_data, sizeof(nv_data));

        print_nv_string(print_char, pfstest_nv_string("    register_test("));
        print_nv_string(print_char, nv_data.name);
        print_nv_string(print_char, pfstest_nv_string(");\n"));
    }
}

void pfstest_print_register_commands(int (*print_char)(int),
                                     pfstest_list_t *before,
                                     pfstest_list_t *after,
                                     pfstest_list_t *suite)
{
    print_register_hook_commands(print_char, before,
                                 pfstest_nv_string("before"));
    print_register_hook_commands(print_char, after,
                                 pfstest_nv_string("after"));
    print_register_test_commands(print_char, suite);
}

void pfstest_print_usage(int (*print_char)(int), char *program_name)
{
    print_nv_string(print_char, pfstest_nv_string("usage: "));
    print_string(print_char, program_name);
    print_nv_string(
        print_char,
        pfstest_nv_string(" [-r] [-v|-x] [-c] [-f source-file] [-n test-name]\n"));
}

static char *next_arg(char **argv[])
{
    char *r = **argv;
    (*argv)++;

    return r;
}

int pfstest_main(int argc, char *argv[])
{
    pfstest_arguments_t args;

    if (pfstest_arguments_parse(&args, argc, argv)) {
        return pfstest_start(stdout_print_char, &args);
    } else {
        pfstest_print_usage(stdout_print_char, args.program_name);
        return 1;
    }
}

bool pfstest_arguments_parse(pfstest_arguments_t *args,
                             int argc, char *argv[])
{
    char *arg;
    char *filter_file;
    char *filter_name;

    args->verbose = false;
    args->xml = false;
    args->color = false;
    args->print_register_commands = false;
    args->filter_file = NULL;
    args->filter_name = NULL;

    if (argc < 1 || argv[0] == NULL) {
        args->program_name = NULL;
        return false;
    }

    args->program_name = next_arg(&argv);

    while (arg = next_arg(&argv), arg != NULL) {
        if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-v"))) {
            args->verbose = true;
            args->xml = false;
        } else if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-x"))) {
            args->xml = true;
            args->verbose = false;
        } else if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-c"))) {
            args->color = true;
        } else if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-r"))) {
            args->print_register_commands = true;
        } else if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-f"))) {
            filter_file = next_arg(&argv);
            if (filter_file == NULL) return false;
            args->filter_file = filter_file;
        } else if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-n"))) {
            filter_name = next_arg(&argv);
            if (filter_name == NULL) return false;
            args->filter_name = filter_name;
            args->verbose = true;
        } else {
            return false;
        }
    }

    return true;
}

int pfstest_start(int (*print_char)(int), pfstest_arguments_t *args)
{
    pfstest_list_t *before = pfstest_suite_get_before_hooks();
    pfstest_list_t *after = pfstest_suite_get_after_hooks();
    pfstest_list_t *suite = pfstest_suite_get_tests();
    
    pfstest_reporter_t *reporter;
    pfstest_report_colorizer_t *colorizer;
    int r;

    if (args->color)
        colorizer = pfstest_report_colorizer_ansi;
    else
        colorizer = pfstest_report_colorizer_null;

    if (args->print_register_commands) {
        pfstest_print_register_commands(print_char, before, after, suite);
        return 0;
    } else {
        if (args->verbose) {
            reporter = pfstest_reporter_verbose_new(print_char, colorizer);
        } else if (args->xml) {
            reporter = pfstest_reporter_xml_new(print_char);
        } else {
            reporter = pfstest_reporter_standard_new(print_char, colorizer);
        }

        r = pfstest_run_registered_tests(
            args->filter_file, args->filter_name, reporter);
        /* Free the memory allocated for the reporter created above */
        pfstest_alloc_free_frame();
        return r;
    }
}
