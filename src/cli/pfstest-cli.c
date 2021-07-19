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

static void reset_args(pfstest_arguments_t *args)
{
    args->program_name = NULL;
    args->verbose = pfstest_false;
    args->xml = pfstest_false;
    args->color = pfstest_false;
    args->print_register_commands = pfstest_false;
    args->filter_file = NULL;
    args->filter_name = NULL;
}

static void sanitize_args(pfstest_arguments_t *args)
{
    if (args->filter_name)
        args->verbose = pfstest_true;
    if (args->xml)
        args->verbose = pfstest_false;
}

static pfstest_bool progname_valid(int argc, char *argv[])
{
    return (argc > 0 && argv[0]);
}

/** Parse a flag that takes no arguments */
static int parse_flag(char *argp[], void *data)
{
    pfstest_bool *flag = data;
    (void)argp;

    *flag = pfstest_true;
    return 1;
}

/** Parse a flag that takes a single string argument */
static int parse_value(char *argp[], void *data)
{
    char *flag_arg = argp[1];
    char **value = data;

    if (!flag_arg)
        return -1;

    *value = flag_arg;
    return 2;
}

typedef struct {
    const pfstest_pg_ptr char *flag;
    int (*parse)(char **, void *);
    size_t offset;
} arg_parser;

static const pfstest_pg char dash_v[] = "-v";
static const pfstest_pg char dash_c[] = "-c";
static const pfstest_pg char dash_x[] = "-x";
static const pfstest_pg char dash_r[] = "-r";
static const pfstest_pg char dash_f[] = "-f";
static const pfstest_pg char dash_n[] = "-n";

#define ARG_OFFSET(field) offsetof(pfstest_arguments_t, field)

static const pfstest_pg arg_parser arg_parsers[] = {
        {dash_v, parse_flag, ARG_OFFSET(verbose)},
        {dash_c, parse_flag, ARG_OFFSET(color)},
        {dash_x, parse_flag, ARG_OFFSET(xml)},
        {dash_r, parse_flag, ARG_OFFSET(print_register_commands)},
        {dash_f, parse_value, ARG_OFFSET(filter_file)},
        {dash_n, parse_value, ARG_OFFSET(filter_name)}
};

static pfstest_bool str_eq_pg_str(char *s1, const pfstest_pg_ptr char *s2)
{
    return (0 == pfstest_strcmp_pg(s1, s2));
}

static int try_parse(arg_parser *p, pfstest_arguments_t *args, char *argp[])
{
    if (!str_eq_pg_str(*argp, p->flag))
        return 0;
    return p->parse(argp, ((char *)args) + p->offset);
}

static pfstest_bool is_parse_error(int r)
{
    return (r < 0);
}

static pfstest_bool is_parse_matched(int r)
{
    return (r > 0);
}

static int args_consumed(int r)
{
    return r;
}

pfstest_bool pfstest_arguments_parse(pfstest_arguments_t *args,
                                     int argc, char *argv[])
{
    size_t num_arg_parsers = sizeof(arg_parsers) / sizeof(arg_parsers[0]);
    arg_parser p;
    size_t i;
    int r;

    reset_args(args);

    if (!progname_valid(argc, argv))
        return pfstest_false;
    args->program_name = *argv++;

    while (*argv) {
        for (i = 0; i < num_arg_parsers; i++) {
            PFSTEST_READ_PG(p, arg_parsers[i]);
            r = try_parse(&p, args, argv);

            if (is_parse_error(r))
                return pfstest_false;

            if (is_parse_matched(r)) {
                argv += args_consumed(r);
                break;
            }
        }
        if (i == num_arg_parsers)
            /* Flag didn't match any parser */
            return pfstest_false;
    }

    sanitize_args(args);

    return pfstest_true;
}
