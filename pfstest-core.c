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

static _pfstest_test_nv_t current_test;
static jmp_buf test_jmp_buf;
static bool fail_expected = false;

const char *program_name = NULL;
static bool verbose = false;

static int passed = 0;
static int failed = 0;
static int ignored = 0;

static pfstest_list_t tests = PFSTEST_LIST_EMPTY();
static pfstest_list_t before = PFSTEST_LIST_EMPTY();
static pfstest_list_t after = PFSTEST_LIST_EMPTY();

static void print_context(void)
{
    pfstest_print_nv_string(current_test.file);
    pfstest_print_nv_string(pfstest_nv_string(":"));
    pfstest_print_nv_string(current_test.name);
    pfstest_print_nv_string(pfstest_nv_string(" "));
}

void pfstest_fail_with_printer(const pfstest_nv_ptr char *file, int line,
                               void (*printer)(const void *),
                               const void *object)
{
    if (fail_expected && !verbose)
        longjmp(test_jmp_buf, RESULT_FAIL);

    if (!verbose) {
        pfstest_print_nv_string(pfstest_nv_string("\n"));
        print_context();
    }

    pfstest_print_nv_string(pfstest_nv_string("FAIL"));
    if (fail_expected)
        pfstest_print_nv_string(pfstest_nv_string(" (expected)"));
    pfstest_print_nv_string(pfstest_nv_string("\n"));
    pfstest_print_nv_string(pfstest_nv_string("    Location: "));
    pfstest_print_nv_string(file);
    pfstest_print_nv_string(pfstest_nv_string(":"));
    pfstest_print_int(line);
    pfstest_print_nv_string(pfstest_nv_string("\n"));
    printer(object);
    pfstest_print_nv_string(pfstest_nv_string("\n"));
    longjmp(test_jmp_buf, RESULT_FAIL);
}

struct message_printer_args
{
    const pfstest_nv_ptr char *message;
};

static void message_printer(const void *object)
{
    const struct message_printer_args *args = object;
    pfstest_print_nv_string(pfstest_nv_string("    "));
    pfstest_print_nv_string(args->message);
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
    longjmp(test_jmp_buf, RESULT_PASS);
}

static void ignore(void)
{
    longjmp(test_jmp_buf, RESULT_IGNORE);
}

static void run_test(void)
{
    if (verbose)
        print_context();
    fflush(stdout);

    fail_expected = current_test.flags & _PFSTEST_FLAG_EXPECT_FAIL;

    switch (setjmp(test_jmp_buf)) {
        case 0:
            if (current_test.flags & _PFSTEST_FLAG_IGNORED) {
                ignore();
            }
            current_test.function();
            pfstest_run_verifiers(); /* TODO: plugin-ize */
            pass();
            break;
        case RESULT_PASS:
            if (fail_expected) {
                fail_expected = false;
                pfstest_fail_at_location(
                    current_test.file, current_test.line,
                    "Test passed when failure expected");
            }

            passed++;
            if (verbose) {
                pfstest_print_nv_string(pfstest_nv_string("PASS\n"));
            } else {
                pfstest_print_nv_string(pfstest_nv_string("."));
                fflush(stdout);
            }
            break;
        case RESULT_FAIL:
            if (fail_expected) {
                fail_expected = false;
                if (verbose)
                    pfstest_print_nv_string(pfstest_nv_string("    "));
                pass();
            }
    
            failed++;
            /* Message already printed before the stack unwound */
            break;
        case RESULT_IGNORE:
            ignored++;
            if (verbose) {
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
    pfstest_list_append(&tests, (pfstest_list_node_t *)the_test);
}

void _pfstest_register_before(pfstest_hook_t *the_hook)
{
    pfstest_list_append(&before, (pfstest_list_node_t *)the_hook);
}

void _pfstest_register_after(pfstest_hook_t *the_hook)
{
    pfstest_list_append(&after, (pfstest_list_node_t *)the_hook);
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
            pfstest_mock_init(); /* TODO: plugin-ize */
            do_hook_list(&before, current_test.file);
            run_test();
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

const char *next_arg(char **argv[], bool required)
{
    char *r = **argv;

    if (r == NULL && required)
        print_usage_and_exit();
    
    (*argv)++;

    return (const char *)r;
}

int pfstest_run_tests(int argc, char *argv[])
{
    const char *test_file = NULL;
    const char *test_name = NULL;
    const char *arg;
    bool register_print = false;

    if (argc == 0 || argv == NULL)
        goto args_done;

    program_name = next_arg(&argv, true);

    while (arg = next_arg(&argv, false), arg != NULL) {
        if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-f"))) {
            test_file = next_arg(&argv, true);
        } else if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-n"))) {
            test_name = next_arg(&argv, true);
            verbose = true;
        } else if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-v"))) {
            verbose = true;
        } else if (0 == pfstest_strcmp_nv(arg, pfstest_nv_string("-r"))) {
            register_print = true;
        } else {
            print_usage_and_exit();
        }
    }
args_done:

    if (register_print) {
        print_register_commands();
        return 0;
    }

    pfstest_print_nv_string(pfstest_nv_string("PFSTest 0.1\n"));
    pfstest_print_nv_string(pfstest_nv_string("===========\n"));
    do_tests_list(test_file, test_name);
    pfstest_printf_nv(
        pfstest_nv_string(
            "\nRun complete. %d passed, %d failed, %d ignored\n"),
        passed, failed, ignored);

    if (failed > 0)
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
