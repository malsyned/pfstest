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

typedef struct 
{
    int passed;
    int failed;
    int ignored;
} results_t;

typedef struct _dynamic_env_t
{
    /* Stack pointer */
    struct _dynamic_env_t *next;

    /* Failure catching */
    jmp_buf test_jmp_buf;

    /* IO Control */
    int (*print_char)(int);
    bool verbose;

    /* File-local context */
    volatile bool test_failed;
    const pfstest_nv_ptr char *test_name;
    const pfstest_nv_ptr char *test_file;
} dynamic_env_t;

static dynamic_env_t *dynamic_env = NULL;

static bool fail_expected = false;

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

void new_pfstest_print_nv_string(int (*print_char)(int),
                                 const pfstest_nv_ptr char *s)
{
    char c;

    while (pfstest_memcpy_nv(&c, s, sizeof(c)), c) {
        print_char(c);
        s++;
    }
}

static void new_pfstest_print_int(int (*print_char)(int), int n)
{
    int d = 1;
    int ntemp = n;

    /* FIXME: Doesn't handle INT_MIN properly (neither does Unity) */
    if (n < 0) {
        print_char('-');
        n = -n;
    }

    while (ntemp > 9) {
        ntemp /= 10;
        d *= 10;
    }

    do {
        print_char('0' + n / d % 10);
        d /= 10;
    } while (d > 0);
}

static void print_context(int (*print_char)(int))
{
    new_pfstest_print_nv_string(print_char, dynamic_env->test_file);
    new_pfstest_print_nv_string(print_char, pfstest_nv_string(":"));
    new_pfstest_print_nv_string(print_char, dynamic_env->test_name);
    new_pfstest_print_nv_string(print_char, pfstest_nv_string(" "));
}

void pfstest_fail_with_printer(const pfstest_nv_ptr char *file, int line,
                               void (*printer)(int (*)(int), const void *),
                               const void *object)
{
    int (*print_char)(int) = dynamic_env->print_char;

    if (fail_expected && !dynamic_env->verbose)
        longjmp(dynamic_env->test_jmp_buf, RESULT_FAIL);

    if (!dynamic_env->verbose) {
        new_pfstest_print_nv_string(print_char, pfstest_nv_string("\n"));
    }
    if (!dynamic_env->verbose || dynamic_env->test_failed) {
        print_context(print_char);
    }

    new_pfstest_print_nv_string(print_char, pfstest_nv_string("FAIL"));
    if (fail_expected) {
        new_pfstest_print_nv_string(print_char,
                                    pfstest_nv_string(" (expected)"));
    }
    new_pfstest_print_nv_string(print_char, pfstest_nv_string("\n"));
    new_pfstest_print_nv_string(print_char,
                                pfstest_nv_string("    Location: "));
    new_pfstest_print_nv_string(print_char, file);
    new_pfstest_print_nv_string(print_char, pfstest_nv_string(":"));
    new_pfstest_print_int(print_char, line);
    new_pfstest_print_nv_string(print_char, pfstest_nv_string("\n"));
    printer(print_char, object);
    new_pfstest_print_nv_string(print_char, pfstest_nv_string("\n"));

    dynamic_env->test_failed = true;

    longjmp(dynamic_env->test_jmp_buf, RESULT_FAIL);
}

struct message_printer_args
{
    const pfstest_nv_ptr char *message;
};

static void message_printer(int (*print_char)(int), const void *object)
{
    const struct message_printer_args *args = object;
    new_pfstest_print_nv_string(print_char, pfstest_nv_string("    "));
    new_pfstest_print_nv_string(print_char, args->message);
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

static void run_test(_pfstest_test_nv_t *current_test, results_t *results)
{
    dynamic_env->test_name = current_test->name;
    dynamic_env->test_file = current_test->file;

    if (dynamic_env->verbose)
        print_context(dynamic_env->print_char);
    fflush(stdout);

    fail_expected = current_test->flags & _PFSTEST_FLAG_EXPECT_FAIL;

    switch (setjmp(dynamic_env->test_jmp_buf)) {
        case 0:
            if (current_test->flags & _PFSTEST_FLAG_IGNORED) {
                ignore();
            }
            current_test->function();
            pfstest_run_verifiers(); /* TODO: plugin-ize */
            pass();
            break;
        case RESULT_PASS:
            if (fail_expected) {
                fail_expected = false;
                pfstest_fail_at_location(
                    current_test->file, current_test->line,
                    "Test passed when failure expected");
            }

            results->passed++;
            if (dynamic_env->verbose) {
                pfstest_print_nv_string(pfstest_nv_string("PASS\n"));
            } else {
                pfstest_print_nv_string(pfstest_nv_string("."));
                fflush(stdout);
            }
            break;
        case RESULT_FAIL:
            if (fail_expected) {
                fail_expected = false;
                if (dynamic_env->verbose)
                    pfstest_print_nv_string(pfstest_nv_string("    "));
                pass();
            }
    
            results->failed++;
            /* Message already printed before the stack unwound */
            break;
        case RESULT_IGNORE:
            results->ignored++;
            if (dynamic_env->verbose) {
                pfstest_print_nv_string(pfstest_nv_string("IGNORED\n"));
            } else {
                pfstest_print_nv_string(pfstest_nv_string("I"));
                fflush(stdout);
            }
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
                          const char *test_name,
                          results_t *results)
{
    _pfstest_test_nv_t current_test;
    pfstest_list_node_t *test_node;

    pfstest_list_iter (test_node, &tests) {
        pfstest_t *test = (pfstest_t *)test_node;
        pfstest_memcpy_nv(&current_test, test->nv_data,
                          sizeof(current_test));

        dynamic_env->test_failed = false;

        if ((test_file == NULL
             || 0 == pfstest_strcmp_nv(test_file, current_test.file))
            && (test_name == NULL
                || 0 == pfstest_strcmp_nv(test_name, current_test.name)))
        {
            pfstest_mock_init(); /* TODO: plugin-ize */
            do_hook_list(&before, current_test.file);
            run_test(&current_test, results);
            do_hook_list(&after, current_test.file);
            pfstest_free_all(); /* TODO: plugin-ize */
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

static const char *next_arg(char **argv[], bool required)
{
    char *r = **argv;

    if (r == NULL && required)
        print_usage_and_exit();
    
    (*argv)++;

    return (const char *)r;
}

int pfstest_run_tests(int argc, char *argv[])
{
    dynamic_env_t local_dynamic_env;
    const char *test_file = NULL;
    const char *test_name = NULL;
    const char *arg;
    bool register_print = false;
    results_t results;

    dynamic_env_push(&local_dynamic_env);

    dynamic_env->print_char = stdout_print_char;
    dynamic_env->verbose = false;

    if (argc == 0 || argv == NULL)
        goto args_done;

    program_name = next_arg(&argv, true);

    while (arg = next_arg(&argv, false), arg != NULL) {
        if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-f"))) {
            test_file = next_arg(&argv, true);
        } else if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-n"))) {
            test_name = next_arg(&argv, true);
            dynamic_env->verbose = true;
        } else if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-v"))) {
            dynamic_env->verbose = true;
        } else if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-r"))) {
            register_print = true;
        } else {
            print_usage_and_exit();
        }
    }
args_done:

    if (register_print) {
        print_register_commands();
        dynamic_env_pop();
        return 0;
    }

    results.passed = results.failed = results.ignored = 0;

    pfstest_print_nv_string(pfstest_nv_string("PFSTest 0.1\n"));
    pfstest_print_nv_string(pfstest_nv_string("===========\n"));
    do_tests_list(test_file, test_name, &results);
    pfstest_printf_nv(
        pfstest_nv_string(
            "\nRun complete. %d passed, %d failed, %d ignored\n"),
        results.passed, results.failed, results.ignored);

    dynamic_env_pop();

    if (results.failed > 0)
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

int pfstest_suite_run(pfstest_list_t *before, pfstest_list_t *after,
                      pfstest_list_t *suite,
                      const pfstest_nv_ptr char *filter_file,
                      const pfstest_nv_ptr char *filter_name,
                      int (*print_char)(int),
                      int flags)
{
    dynamic_env_t local_dynamic_env;
    pfstest_list_node_t *test_node;
    pfstest_list_node_t *hook_node;
    _pfstest_test_nv_t current_test;
    _pfstest_hook_nv_t current_hook;
    results_t results;
    results.passed = results.failed = results.ignored = 0;

    dynamic_env_push(&local_dynamic_env);
    dynamic_env->print_char = print_char;
    dynamic_env->verbose = ((flags & PFSTEST_SUITE_RUN_FLAG_VERBOSE)
                            ? true : false);

    pfstest_list_iter (test_node, suite) {
        pfstest_t *the_test = (pfstest_t *)test_node;
        pfstest_memcpy_nv(&current_test, the_test->nv_data,
                          sizeof(current_test));

        if ((filter_name != NULL
             && 0 != pfstest_strcmp_nvnv(filter_name, current_test.name))
            || (filter_file != NULL
                && 0 != pfstest_strcmp_nvnv(filter_file, current_test.file)))
        {
            continue;
        }

        dynamic_env->test_name = current_test.name;
        dynamic_env->test_file = current_test.file;
        dynamic_env->test_failed = false;

        if (dynamic_env->verbose)
            print_context(print_char);

        if (current_test.flags & _PFSTEST_FLAG_IGNORED) {
            results.ignored++;
            if (dynamic_env->verbose) {
                new_pfstest_print_nv_string(print_char,
                                            pfstest_nv_string("IGNORED\n"));
            } else {
                print_char('I');
            }
            continue;
        }

        /* TODO: Allow filtering by file name */
        /* TODO: Start a pfstest_alloc dynamic frame */
        /* TODO: pfstest_mock_init() in a new dynamic frame */
        
        if (0 == setjmp(dynamic_env->test_jmp_buf)) {
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

            current_test.function();
        }

        pfstest_list_iter(hook_node, after) {
            pfstest_hook_t *after_hook = (pfstest_hook_t *)hook_node;
            pfstest_memcpy_nv(&current_hook, after_hook->nv_data,
                              sizeof(current_hook));

            if (0 == setjmp(dynamic_env->test_jmp_buf)) {
                if (0 == pfstest_strcmp_nvnv(current_test.file,
                                             current_hook.file))
                {
                    current_hook.function();
                }
            }
        }

        if (dynamic_env->test_failed) {
            results.failed++;
        } else {
            results.passed++;
            if (dynamic_env->verbose) {
                new_pfstest_print_nv_string(print_char,
                                            pfstest_nv_string("PASS\n"));
            } else {
                print_char('.');
            }
        }
        /* TODO: pop pfstest_mock frame */
        /* TODO: pop pfstest_alloc frame */
    }

    new_pfstest_print_nv_string(print_char,
                                pfstest_nv_string("\nRun complete. "));
    new_pfstest_print_int(print_char, results.passed);
    new_pfstest_print_nv_string(print_char,
                                pfstest_nv_string(" passed, "));
    new_pfstest_print_int(print_char, results.failed);
    new_pfstest_print_nv_string(print_char,
                                pfstest_nv_string(" failed, "));
    new_pfstest_print_int(print_char, results.ignored);
    new_pfstest_print_nv_string(print_char,
                                pfstest_nv_string(" ignored\n"));

    dynamic_env_pop();

    return results.failed;
}

void _pfstest_hook_list_register_hook(pfstest_list_t *list,
                                      pfstest_hook_t *hook)
{
    pfstest_list_node_init((pfstest_list_node_t *)hook);
    pfstest_list_append(list, (pfstest_list_node_t *)hook);
}
