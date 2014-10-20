#include "pfstest-core.h"

#include <setjmp.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* TODO: plugin-ize */
#include "pfstest-alloc.h"
#include "pfstest-mock.h"

#define RESULT_PASS 1
#define RESULT_FAIL 2
#define RESULT_IGNORE 3

static pfstest_t *current_test;
static jmp_buf test_jmp_buf;
static bool fail_expected;

const char *program_name = NULL;
static bool verbose;

static int passed;
static int failed;
static int ignored;

static pfstest_list_t tests = PFSTEST_LIST_EMPTY();
static pfstest_list_t before = PFSTEST_LIST_EMPTY();
static pfstest_list_t after = PFSTEST_LIST_EMPTY();

static void print_context(pfstest_t *the_test)
{
    printf("%s:%s ", the_test->file, the_test->name);
}

void pfstest_fail_with_printer(const char *file, int line,
                               void (*printer)(const void *),
                               const void *object)
{
    if (fail_expected && !verbose)
        longjmp(test_jmp_buf, RESULT_FAIL);

    if (!verbose) {
        printf("\n");
        print_context(current_test);
    }

    printf("FAIL");
    if (fail_expected)
        printf(" (expected)");
    printf("\n");
    printf("    Location: %s:%d\n", file, line);
    printer(object);
    printf("\n");
    longjmp(test_jmp_buf, RESULT_FAIL);
}

static void message_printer(const void *object)
{
    const char *message = object;
    printf("    %s", message);
}

void pfstest_fail_at_location(const char *file, int line,
                              const char *message)
{
    pfstest_fail_with_printer(file, line, message_printer, message);
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
        print_context(current_test);
    fflush(stdout);

    fail_expected = (current_test->flags & _PFSTEST_FLAG_EXPECT_FAIL);

    switch (setjmp(test_jmp_buf)) {
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

            passed++;
            if (verbose)
                printf("PASS\n");
            else
                printf("."); fflush(stdout);
            break;
        case RESULT_FAIL:
            if (fail_expected) {
                fail_expected = false;
                if (verbose)
                    printf("    ");
                pass();
            }
    
            failed++;
            /* Message already printed before the stack unwound */
            break;
        case RESULT_IGNORE:
            ignored++;
            if (verbose)
                printf("IGNORED\n");
            else
                printf("I"); fflush(stdout);
            break;
        default:
            printf("FATAL ERROR\n");
            exit(1);
            break;
    }
}

void pfstest_register_test(pfstest_t *the_test)
{
    pfstest_list_append(&tests, (pfstest_list_node_t *)the_test);
}

void pfstest_register_before(pfstest_hook_t *the_hook)
{
    pfstest_list_append(&before, (pfstest_list_node_t *)the_hook);
}

void pfstest_register_after(pfstest_hook_t *the_hook)
{
    pfstest_list_append(&after, (pfstest_list_node_t *)the_hook);
}

static void do_hook_list(pfstest_list_t *list, const char *file)
{
    pfstest_list_node_t *hook_node;

    pfstest_list_iter (hook_node, list) {
        pfstest_hook_t *hook = (pfstest_hook_t *)hook_node;
        if (file == NULL || 0 == strcmp(hook->file, file))
            hook->function();
    }
}

static void do_tests_list(const char *test_file,
                          const char *test_name)
{
    pfstest_list_node_t *test_node;

    pfstest_list_iter (test_node, &tests) {
        pfstest_t *test = (pfstest_t *)test_node;
        if ((test_file == NULL || 0 == strcmp(test_file, test->file))
            && (test_name == NULL || 0 == strcmp(test_name, test->name)))
        {
            current_test = test;
            pfstest_mock_init(); /* TODO: plugin-ize */
            do_hook_list(&before, test->file);
            run_test();
            do_hook_list(&after, test->file);
            pfstest_free_all(); /* TODO: plugin-ize */
        }
    }
}

static void print_usage_and_exit(void)
{
    printf("usage: %s [-v] [-f source-file] [-n test-name]\n", program_name);
    exit(1);
}

char *next_arg(char **argv[], bool required)
{
    char *r = **argv;

    if (r == NULL && required)
        print_usage_and_exit();
    
    (*argv)++;

    return r;
}

int pfstest_run_tests(int argc, char *argv[])
{
    const char *test_file = NULL;
    const char *test_name = NULL;
    char *arg;

    if (argc == 0 || argv == NULL)
        goto args_done;

    program_name = next_arg(&argv, true);

    while (arg = next_arg(&argv, false), arg != NULL) {
        if (0 == strcmp("-f", arg)) {
            test_file = next_arg(&argv, true);
        } else if (0 == strcmp("-n", arg)) {
            test_name = next_arg(&argv, true);
            verbose = true;
        } else if (0 == strcmp("-v", arg)) {
            verbose = true;
        } else {
            print_usage_and_exit();
        }
    }
args_done:

    printf("PFSTest 0.1\n");
    printf("===========\n");
    do_tests_list(test_file, test_name);
    printf("\nRun complete. %d passed, %d failed, %d ignored\n",
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
    char *argv[] = {"", "-v", NULL};

    return run_tests(2, argv);
}
