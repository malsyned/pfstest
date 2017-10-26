#include "pfstest-cli.h"

#include <stdio.h>

#include "pfstest-core.h"
#include "pfstest-reporters-standard.h"
#include "pfstest-reporter-xml.h"
#include "pfstest-alloc.h"

static void print_pg_string(int (*print_char)(int),
                            const pfstest_pg_ptr char *s)
{
    char c;

    while (pfstest_memcpy_pg(&c, s, sizeof(c)), c) {
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

static void print_register_plugin_commands(int (*print_char)(int),
                                           pfstest_list_t *plugins)
{
    pfstest_plugin_t *plugin;
    _pfstest_plugin_pg_t pg_data;

    pfstest_list_iter (plugin, plugins) {
        pfstest_memcpy_pg(&pg_data, plugin->pg_data, sizeof(pg_data));

        print_pg_string(print_char,
                        pfstest_pg_string("    register_plugin("));
        print_pg_string(print_char, pg_data.name);
        print_pg_string(print_char, pfstest_pg_string(");\n"));
    }
}

static void print_register_hook_commands(
    int (*print_char)(int), pfstest_list_t *list,
    const pfstest_pg_ptr char *list_name)
{
    pfstest_hook_t *hook;
    _pfstest_hook_pg_t pg_data;

    pfstest_list_iter (hook, list) {
        pfstest_memcpy_pg(&pg_data, hook->pg_data, sizeof(pg_data));

        print_pg_string(print_char, pfstest_pg_string("    register_"));
        print_pg_string(print_char, list_name);
        print_pg_string(print_char, pfstest_pg_string("("));
        print_pg_string(print_char, pg_data.name);
        print_pg_string(print_char, pfstest_pg_string(");\n"));
    }
}

static void print_register_test_commands(int (*print_char)(int),
                                         pfstest_list_t *tests)
{
    pfstest_t *test;
    _pfstest_test_pg_t pg_data;

    pfstest_list_iter (test, tests) {
        pfstest_memcpy_pg(&pg_data, test->pg_data, sizeof(pg_data));

        print_pg_string(print_char, pfstest_pg_string("    register_test("));
        print_pg_string(print_char, pg_data.name);
        print_pg_string(print_char, pfstest_pg_string(");\n"));
    }
}

void pfstest_print_register_commands(int (*print_char)(int),
                                     pfstest_list_t *before,
                                     pfstest_list_t *after,
                                     pfstest_list_t *plugins,
                                     pfstest_list_t *suite)
{
    print_register_plugin_commands(print_char, plugins);
    print_register_hook_commands(print_char, before,
                                 pfstest_pg_string("before"));
    print_register_hook_commands(print_char, after,
                                 pfstest_pg_string("after"));
    print_register_test_commands(print_char, suite);
}

#define USAGE_ARGS "[-r] [-v|-x] [-c] [-f source-file] [-n test-name]\n"

void pfstest_print_usage(int (*print_char)(int), char *program_name)
{
    print_pg_string(print_char, pfstest_pg_string("usage: "));
    if (program_name != NULL) {
        print_string(print_char, program_name);
        print_pg_string(print_char, pfstest_pg_string(" "));
    }
    print_pg_string(print_char, pfstest_pg_string(USAGE_ARGS));
}

static void registered_tests_print_register_commands(int (*print_char)(int))
{
    pfstest_list_t *plugins = pfstest_get_registered_plugins();
    pfstest_list_t *before = pfstest_get_registered_before_hooks();
    pfstest_list_t *after = pfstest_get_registered_after_hooks();
    pfstest_list_t *suite = pfstest_get_registered_tests();

    pfstest_print_register_commands(print_char, before, after,
                                    plugins, suite);
}

static pfstest_report_colorizer_t *select_colorizer(pfstest_arguments_t *args)
{
    if (args->color)
        return pfstest_report_colorizer_ansi;
    else
        return pfstest_report_colorizer_null;
}

static pfstest_reporter_t *create_selected_reporter(
    int (*print_char)(int),
    pfstest_arguments_t *args,
    pfstest_report_colorizer_t *colorizer)
{
    if (args->verbose) {
        return pfstest_reporter_verbose_new(print_char, colorizer);
    } else if (args->xml) {
        return pfstest_reporter_xml_new(print_char);
    } else {
        return pfstest_reporter_standard_new(print_char, colorizer);
    }
}

static int pfstest_start_with_args(int (*print_char)(int),
                                   pfstest_arguments_t *args)
{
    pfstest_reporter_t *reporter;
    pfstest_report_colorizer_t *colorizer;
    int r;

    colorizer = select_colorizer(args);
    reporter = create_selected_reporter(print_char, args, colorizer);

    r = pfstest_run_registered_tests(
        args->filter_file, args->filter_name, reporter);
    /* Free the memory allocated for the reporter created above */
    pfstest_alloc_free_frame();
    return r;
}

int pfstest_main(int argc, char *argv[])
{
    pfstest_arguments_t args;

    if (!pfstest_arguments_parse(&args, argc, argv)) {
        pfstest_print_usage(stdout_print_char, args.program_name);
        return EXIT_FAILURE;
    } else if (args.print_register_commands) {
        registered_tests_print_register_commands(stdout_print_char);
        return EXIT_SUCCESS;
    } else {
        return pfstest_start_with_args(stdout_print_char, &args);
    }
}

static char *next_arg(char **argv[])
{
    char *r = **argv;
    (*argv)++;

    return r;
}

static pfstest_bool str_is_pg_str(char *s1, const pfstest_pg_ptr char *s2)
{
    return (0 == pfstest_strcmp_pg(s1, s2));
}

pfstest_bool pfstest_arguments_parse(pfstest_arguments_t *args,
                                     int argc, char *argv[])
{
    char *arg;
    char *filter_file;
    char *filter_name;

    args->verbose = pfstest_false;
    args->xml = pfstest_false;
    args->color = pfstest_false;
    args->print_register_commands = pfstest_false;
    args->filter_file = NULL;
    args->filter_name = NULL;

    if (argc < 1 || argv[0] == NULL) {
        args->program_name = NULL;
        return pfstest_false;
    }

    args->program_name = next_arg(&argv);

    while (arg = next_arg(&argv), arg != NULL) {
        if (str_is_pg_str(arg, pfstest_pg_string("-v"))) {
            args->verbose = pfstest_true;
            args->xml = pfstest_false;
        } else if (str_is_pg_str(arg, pfstest_pg_string("-x"))) {
            args->xml = pfstest_true;
            args->verbose = pfstest_false;
        } else if (str_is_pg_str(arg, pfstest_pg_string("-c"))) {
            args->color = pfstest_true;
        } else if (str_is_pg_str(arg, pfstest_pg_string("-r"))) {
            args->print_register_commands = pfstest_true;
        } else if (str_is_pg_str(arg, pfstest_pg_string("-f"))) {
            filter_file = next_arg(&argv);
            if (filter_file == NULL) return pfstest_false;
            args->filter_file = filter_file;
        } else if (str_is_pg_str(arg, pfstest_pg_string("-n"))) {
            filter_name = next_arg(&argv);
            if (filter_name == NULL) return pfstest_false;
            args->filter_name = filter_name;
            args->verbose = pfstest_true;
        } else {
            return pfstest_false;
        }
    }

    return pfstest_true;
}
