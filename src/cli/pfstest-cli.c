/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest-cli.h"

#include <stdio.h>

#include "pfstest-core.h"
#include "pfstest-reporters-standard.h"
#include "pfstest-reporter-xml.h"
#include "pfstest-alloc.h"

static void print_pg_str(int (*char_writer)(int),
                         const pfstest_pg_ptr char *s)
{
    char c;

    while (PFSTEST_READ_RETURN_PG(c, *s++)) {
        char_writer(c);
    }
}

static void print_string(int (*char_writer)(int), const char *s)
{
    while (*s) {
        char_writer(*s++);
    }
}

static int stdout_char_writer(int c)
{
    int r = putc(c, stdout);
    fflush(stdout);
    return r;
}

static void print_register_plugin_commands(int (*char_writer)(int),
                                           pfstest_list_t *plugins)
{
    pfstest_plugin_t *plugin;
    _pfstest_plugin_pg_t pg_data;

    pfstest_list_iter (plugin, plugins) {
        PFSTEST_READ_PG(pg_data, *plugin->pg_data);

        print_pg_str(char_writer, pfstest_pg_str("    register_plugin("));
        print_pg_str(char_writer, pg_data.name);
        print_pg_str(char_writer, pfstest_pg_str(");\n"));
    }
}

static void print_register_test_commands(int (*char_writer)(int),
                                         pfstest_list_t *tests)
{
    pfstest_t *test;
    _pfstest_test_pg_t pg_data;

    pfstest_list_iter (test, tests) {
        PFSTEST_READ_PG(pg_data, *test->pg_data);

        print_pg_str(char_writer, pfstest_pg_str("    register_test("));
        print_pg_str(char_writer, pg_data.name);
        print_pg_str(char_writer, pfstest_pg_str(");\n"));
    }
}

void pfstest_print_register_commands(int (*char_writer)(int),
                                     pfstest_list_t *plugins,
                                     pfstest_list_t *suite)
{
    print_register_plugin_commands(char_writer, plugins);
    print_register_test_commands(char_writer, suite);
}

#define USAGE_ARGS "[-r] [-v|-x] [-c] [-f source-file] [-n test-name]\n"

void pfstest_print_usage(int (*char_writer)(int), char *program_name)
{
    print_pg_str(char_writer, pfstest_pg_str("usage: "));
    if (program_name != NULL) {
        print_string(char_writer, program_name);
        print_pg_str(char_writer, pfstest_pg_str(" "));
    }
    print_pg_str(char_writer, pfstest_pg_str(USAGE_ARGS));
}

static void registered_tests_print_register_commands(int (*char_writer)(int))
{
    pfstest_list_t *plugins = pfstest_get_registered_plugins();
    pfstest_list_t *suite = pfstest_get_registered_tests();

    pfstest_print_register_commands(char_writer, plugins, suite);
}

static const pfstest_pg_ptr
pfstest_report_colorizer_t *select_colorizer(pfstest_arguments_t *args)
{
    if (args->color)
        return pfstest_report_colorizer_ansi;
    else
        return pfstest_report_colorizer_null;
}

static pfstest_reporter_t *create_selected_reporter(
    int (*char_writer)(int),
    pfstest_arguments_t *args,
    const pfstest_pg_ptr pfstest_report_colorizer_t *colorizer)
{
    if (args->verbose) {
        return pfstest_reporter_verbose_new(char_writer, colorizer);
    } else if (args->xml) {
        return pfstest_reporter_xml_new(char_writer);
    } else {
        return pfstest_reporter_standard_new(char_writer, colorizer);
    }
}

static int pfstest_start_with_args(int (*char_writer)(int),
                                   pfstest_arguments_t *args)
{
    pfstest_reporter_t *reporter;
    const pfstest_pg_ptr pfstest_report_colorizer_t *colorizer;
    int r;

    colorizer = select_colorizer(args);
    reporter = create_selected_reporter(char_writer, args, colorizer);

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
        pfstest_print_usage(stdout_char_writer, args.program_name);
        return EXIT_FAILURE;
    } else if (args.print_register_commands) {
        registered_tests_print_register_commands(stdout_char_writer);
        return EXIT_SUCCESS;
    } else {
        return pfstest_start_with_args(stdout_char_writer, &args);
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
        if (str_is_pg_str(arg, pfstest_pg_str("-v"))) {
            args->verbose = pfstest_true;
            args->xml = pfstest_false;
        } else if (str_is_pg_str(arg, pfstest_pg_str("-x"))) {
            args->xml = pfstest_true;
            args->verbose = pfstest_false;
        } else if (str_is_pg_str(arg, pfstest_pg_str("-c"))) {
            args->color = pfstest_true;
        } else if (str_is_pg_str(arg, pfstest_pg_str("-r"))) {
            args->print_register_commands = pfstest_true;
        } else if (str_is_pg_str(arg, pfstest_pg_str("-f"))) {
            filter_file = next_arg(&argv);
            if (filter_file == NULL) return pfstest_false;
            args->filter_file = filter_file;
        } else if (str_is_pg_str(arg, pfstest_pg_str("-n"))) {
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
