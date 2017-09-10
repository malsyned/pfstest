#include "pfstest-core.h"

#include <setjmp.h>
#include <stdio.h>
#include <string.h>

#include "pfstest-platform.h"
#include "pfstest-output-formatter-xml.h"
/* TODO: plugin-ize */
#include "pfstest-alloc.h"
#include "pfstest-mock.h"

#define RESULT_PASS 1
#define RESULT_FAIL 2
#define RESULT_IGNORE 3

typedef struct _dynamic_env_t
{
    /* Stack pointer */
    struct _dynamic_env_t *next;

    /* Failure catching */
    jmp_buf test_jmp_buf;

    /* IO Control */
    pfstest_output_formatter_t *formatter;
} dynamic_env_t;

static dynamic_env_t *dynamic_env = NULL;

static pfstest_list_t tests = PFSTEST_LIST_EMPTY();
static pfstest_list_t before = PFSTEST_LIST_EMPTY();
static pfstest_list_t after = PFSTEST_LIST_EMPTY();

static void dynamic_env_push(dynamic_env_t *new_env)
{
    new_env->next = dynamic_env;
    dynamic_env = new_env;
}

static void dynamic_env_pop(void)
{
    dynamic_env = dynamic_env->next;
}

static int stdout_print_char(int c)
{
    int r = fputc(c, stdout);
    fflush(stdout);
    return r;
}

void pfstest_fail_with_printer(
    const pfstest_nv_ptr char *file, int line,
    void (*printer)(pfstest_output_formatter_t *formatter,
                    const void *),
    const void *object)
{
    pfstest_output_formatter_t *formatter = dynamic_env->formatter;

    pfstest_output_formatter_test_failed_message_start(
        formatter, file, line);

    printer(formatter, object);

    pfstest_output_formatter_test_failed_message_complete(formatter);

    longjmp(dynamic_env->test_jmp_buf, RESULT_FAIL);
}

struct message_printer_args
{
    const pfstest_nv_ptr char *message;
};

static void message_printer(pfstest_output_formatter_t *formatter,
                            const void *object)
{
    const struct message_printer_args *args = object;
    pfstest_output_formatter_print_nv_string(formatter, args->message);
}

void _pfstest_fail_at_location(
    const pfstest_nv_ptr char *file, int line,
    const pfstest_nv_ptr char *message)
{
    struct message_printer_args args;

    args.message = message;

    pfstest_fail_with_printer(file, line, message_printer,
                              (const void *)&args);
}

void _pfstest_register_test(pfstest_t *the_test)
{
    _pfstest_suite_register_test(&tests, the_test);
}

void _pfstest_register_before(pfstest_hook_t *the_hook)
{
    _pfstest_hook_list_register_hook(&before, the_hook);
}

void _pfstest_register_after(pfstest_hook_t *the_hook)
{
    _pfstest_hook_list_register_hook(&after, the_hook);
}

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
        pfstest_nv_string(" [-r] [-v] [-f source-file] [-n test-name]\n"));
}

void _pfstest_suite_register_test(pfstest_list_t *suite,
                                  pfstest_t *the_test)
{
    pfstest_list_node_init((pfstest_list_node_t *)the_test);
    pfstest_list_append(suite, (pfstest_list_node_t *)the_test);
}

void pfstest_run(pfstest_t *the_test,
                 pfstest_list_t *before, pfstest_list_t *after,
                 const pfstest_nv_ptr char *filter_file,
                 const pfstest_nv_ptr char *filter_name,
                 pfstest_output_formatter_t *formatter)
{
    dynamic_env_t local_dynamic_env;
    _pfstest_test_nv_t current_test;
    pfstest_list_node_t *hook_node;
    _pfstest_hook_nv_t current_hook;

    dynamic_env_push(&local_dynamic_env);
    dynamic_env->formatter = formatter;

    pfstest_memcpy_nv(&current_test, the_test->nv_data,
                      sizeof(current_test));

    if ((filter_name != NULL
         && 0 != pfstest_strcmp_nvnv(filter_name, current_test.name))
        || (filter_file != NULL
            && 0 != pfstest_strcmp_nvnv(filter_file, current_test.file)))
    {
        goto cleanup;
    }

    pfstest_output_formatter_test_started(formatter,
                                          current_test.name,
                                          current_test.file);

    if (current_test.flags & _PFSTEST_FLAG_IGNORED) {
        pfstest_output_formatter_test_ignored(formatter);
        goto finish;
    }

    pfstest_alloc_frame_push();

    pfstest_mock_init();
        
    if (0 == setjmp(dynamic_env->test_jmp_buf)) {
        if (before != NULL) {
            pfstest_list_iter(hook_node, before) {
                pfstest_hook_t *before_hook =
                    (pfstest_hook_t *)hook_node;
                pfstest_memcpy_nv(&current_hook, before_hook->nv_data,
                                  sizeof(current_hook));
        
                if (0 == pfstest_strcmp_nvnv(current_test.file,
                                             current_hook.file))
                {
                    current_hook.function();
                }
            }
        }

        current_test.function();
        pfstest_run_verifiers(); /* TODO: plugin-ize */
    }
    pfstest_mock_finish(); /* TODO: plugin-ize */

    if (after != NULL) {
        pfstest_list_iter(hook_node, after) {
            pfstest_hook_t *after_hook = (pfstest_hook_t *)hook_node;
            pfstest_memcpy_nv(&current_hook, after_hook->nv_data,
                              sizeof(current_hook));

            if (0 == pfstest_strcmp_nvnv(current_test.file,
                                         current_hook.file))
            {
                if (0 == setjmp(dynamic_env->test_jmp_buf)) {
                    current_hook.function();
                }
            }
        }
    }

    pfstest_alloc_free_frame();
    pfstest_alloc_frame_pop();

finish:
    pfstest_output_formatter_test_complete(formatter);

cleanup:
    dynamic_env_pop();
}

int pfstest_suite_run(pfstest_list_t *before, pfstest_list_t *after,
                      pfstest_list_t *suite,
                      const pfstest_nv_ptr char *filter_file,
                      const pfstest_nv_ptr char *filter_name,
                      pfstest_output_formatter_t *formatter)
{
    pfstest_list_node_t *test_node;
    int result;

    pfstest_output_formatter_run_started(formatter);

    pfstest_list_iter (test_node, suite) {
        pfstest_t *the_test = (pfstest_t *)test_node;

        pfstest_run(the_test, before, after,
                    filter_file, filter_name, formatter);
    }

    pfstest_output_formatter_run_complete(formatter);

    result = pfstest_output_formatter_return_value(formatter);

    return result;
}

void _pfstest_hook_list_register_hook(pfstest_list_t *list,
                                      pfstest_hook_t *hook)
{
    pfstest_list_node_init((pfstest_list_node_t *)hook);
    pfstest_list_append(list, (pfstest_list_node_t *)hook);
}

static char *next_arg(char **argv[])
{
    char *r = **argv;
    (*argv)++;

    return r;
}

bool pfstest_arguments_parse(pfstest_arguments_t *args,
                             int argc, char *argv[])
{
    char *arg;
    char *filter_file;
    char *filter_name;

    args->verbose = false;
    args->xml = false;
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
    pfstest_output_formatter_t *formatter;
    int r;

    if (args->print_register_commands) {
        pfstest_print_register_commands(print_char, &before, &after, &tests);
        return 0;
    } else {
        if (args->verbose) {
            formatter = pfstest_output_formatter_verbose_new(print_char);
        } else if (args->xml) {
            formatter = pfstest_output_formatter_xml_new(print_char);
        } else {
            formatter = pfstest_output_formatter_standard_new(print_char);
        }

        r = pfstest_suite_run(&before, &after, &tests,
                              args->filter_file, args->filter_name,
                              formatter);
        pfstest_alloc_free_frame();
        return r;
    }
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
