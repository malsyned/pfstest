#include "pfstest-core.h"

#include <setjmp.h>
#include <stdio.h>
#include <string.h>

#include "pfstest-platform.h"
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

const char *program_name = NULL;

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
    pfstest_output_formatter_message_print_nv_string(formatter,
                                                     args->message);
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

static void pass(void)
{
    longjmp(dynamic_env->test_jmp_buf, RESULT_PASS);
}

static void ignore(void)
{
    longjmp(dynamic_env->test_jmp_buf, RESULT_IGNORE);
}

static void run_test(_pfstest_test_nv_t *current_test)
{
    switch (setjmp(dynamic_env->test_jmp_buf)) {
        case 0:
            if (current_test->flags & _PFSTEST_FLAG_IGNORED) {
                ignore();
            }
            current_test->function();
            pfstest_run_verifiers();
            pass();
            break;
        case RESULT_PASS:
            pfstest_output_formatter_test_complete(dynamic_env->formatter);
            break;
        case RESULT_FAIL:
            pfstest_output_formatter_test_complete(dynamic_env->formatter);
            /* Message already printed before the stack unwound */
            break;
        case RESULT_IGNORE:
            pfstest_output_formatter_test_ignored(dynamic_env->formatter);
            pfstest_output_formatter_test_complete(dynamic_env->formatter);
            break;
        default:
            pfstest_print_nv_string(pfstest_nv_string("FATAL ERROR\n"));
            exit(1);
            break;
    }
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

static void do_hook_list(pfstest_list_t *list,
                         const pfstest_nv_ptr char *file)
{
    pfstest_list_node_t *hook_node;
    _pfstest_hook_nv_t nv_data;

    pfstest_list_iter (hook_node, list) {
        pfstest_hook_t *hook = (pfstest_hook_t *)hook_node;
        pfstest_memcpy_nv(&nv_data, hook->nv_data, sizeof(nv_data));

        if (file == NULL || 0 == pfstest_strcmp_nvnv(file, nv_data.file))
            nv_data.function();
    }
}

static void do_tests_list(const char *test_file,
                          const char *test_name)
{
    _pfstest_test_nv_t current_test;
    pfstest_list_node_t *test_node;

    pfstest_list_iter (test_node, &tests) {
        pfstest_t *test = (pfstest_t *)test_node;
        pfstest_memcpy_nv(&current_test, test->nv_data,
                          sizeof(current_test));

        if ((test_file == NULL
             || 0 == pfstest_strcmp_nv(test_file, current_test.file))
            && (test_name == NULL
                || 0 == pfstest_strcmp_nv(test_name, current_test.name)))
        {
            pfstest_output_formatter_test_started(dynamic_env->formatter,
                                                  current_test.name,
                                                  current_test.file);

            pfstest_alloc_frame_push();

            pfstest_mock_init(); /* TODO: plugin-ize */
            do_hook_list(&before, current_test.file);
            run_test(&current_test);
            do_hook_list(&after, current_test.file);

            pfstest_mock_finish(); /* TODO: plugin-ize */
            pfstest_alloc_free_frame();
            pfstest_alloc_frame_pop();
        }
    }
}

static void print_register_hook_commands(
    pfstest_list_t *list, const pfstest_nv_ptr char *list_name)
{
    pfstest_list_node_t *hook_node;
    _pfstest_hook_nv_t nv_data;

    pfstest_list_iter (hook_node, list) {
        pfstest_hook_t *hook = (pfstest_hook_t *)hook_node;
        pfstest_memcpy_nv(&nv_data, hook->nv_data, sizeof(nv_data));

        pfstest_print_nv_string(pfstest_nv_string("    register_"));
        pfstest_print_nv_string(list_name);
        pfstest_print_nv_string(pfstest_nv_string("("));
        pfstest_print_nv_string(nv_data.name);
        pfstest_print_nv_string(pfstest_nv_string(");\n"));
    }
}

static void print_register_test_commands(void)
{
    pfstest_list_node_t *test_node;
    _pfstest_test_nv_t nv_data;

    pfstest_list_iter (test_node, &tests) {
        pfstest_t *test = (pfstest_t *)test_node;
        pfstest_memcpy_nv(&nv_data, test->nv_data, sizeof(nv_data));

        pfstest_print_nv_string(pfstest_nv_string("    register_test("));
        pfstest_print_nv_string(nv_data.name);
        pfstest_print_nv_string(pfstest_nv_string(");\n"));
    }
}

static void print_register_commands(void)
{
    print_register_hook_commands(&before, pfstest_nv_string("before"));
    print_register_hook_commands(&after, pfstest_nv_string("after"));
    print_register_test_commands();
}

static void print_usage_and_exit(void)
{
    pfstest_printf_nv(
        pfstest_nv_string(
            "usage: %s [-r] [-v] [-f source-file] [-n test-name]\n"),
        program_name);
    exit(1);
}

int pfstest_run_tests(int argc, char *argv[])
{
    dynamic_env_t local_dynamic_env;
    pfstest_arguments_t args;
    bool args_parse_good;
    pfstest_output_formatter_t formatter;
    int result;

    dynamic_env_push(&local_dynamic_env);

    args_parse_good = pfstest_arguments_parse(&args, argc, argv);

    program_name = args.program_name;

    if (!args_parse_good) {
        print_usage_and_exit();
    }

    if (args.verbose) {
        pfstest_output_formatter_verbose_init(&formatter,
                                              stdout_print_char);
    } else {
        pfstest_output_formatter_standard_init(&formatter,
                                               stdout_print_char);
    }
    dynamic_env->formatter = &formatter;

    if (args.print_register_commands) {
        print_register_commands();
        dynamic_env_pop();
        return 0;
    }

    pfstest_print_nv_string(pfstest_nv_string("PFSTest 0.1\n"));
    pfstest_print_nv_string(pfstest_nv_string("===========\n"));

    pfstest_output_formatter_run_started(&formatter);
    do_tests_list(args.filter_file, args.filter_name);
    pfstest_output_formatter_run_complete(&formatter);

    result = pfstest_output_formatter_return_value(&formatter);

    dynamic_env_pop();

    if (result > 0)
        return 1;
    else
        return 0;
}

int pfstest_run_all_tests(void)
{
    return run_tests(0, NULL);
}

int pfstest_run_all_tests_verbose(void)
{
    /* mcc18 stores string constants in ROM, which is a separate
     * memory space, unless you explicitly place them to RAM like
     * this.  */
    char argv_0[] = "";
    char argv_1[] = "-v";
    char *argv[3];

    argv[0] = argv_0;
    argv[1] = argv_1;
    argv[2] = NULL;

    return run_tests(2, argv);
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

    /* TODO: pfstest_mock_init() in a new dynamic frame */
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
        /* TODO: pop pfstest_mock frame */
        pfstest_run_verifiers();
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

int run_all_tests_new(void)
{
    pfstest_output_formatter_t formatter;
    int result;

    pfstest_output_formatter_verbose_init(&formatter, stdout_print_char);

    pfstest_print_nv_string(pfstest_nv_string("PFSTest 0.1\n"));
    pfstest_print_nv_string(pfstest_nv_string("===========\n"));

    result = pfstest_suite_run(&before, &after, &tests, NULL, NULL,
                               &formatter);

    return result;
}
