#include "pfstest-core.h"

#include <setjmp.h>
#include <string.h>

#include "pfstest-platform.h"
#include "pfstest-basename.h"
#include "pfstest-reporter-xml.h"
/* TODO: plugin-ize */
#include "pfstest-alloc.h"
#include "pfstest-mock.h"

#define RESULT_FAIL 1

typedef struct _dynamic_env_t
{
    /* Stack pointer */
    struct _dynamic_env_t *next;

    /* Failure catching */
    jmp_buf test_jmp_buf;

    /* IO Control */
    pfstest_reporter_t *reporter;
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

void pfstest_fail_with_printer(
    const pfstest_nv_ptr char *file, int line,
    void (*printer)(pfstest_reporter_t *reporter, const void *),
    const void *object)
{
    pfstest_reporter_t *reporter = dynamic_env->reporter;

    pfstest_reporter_test_failed_message_start(reporter, file, line);
    printer(reporter, object);
    pfstest_reporter_test_failed_message_complete(reporter);

    longjmp(dynamic_env->test_jmp_buf, RESULT_FAIL);
}

struct message_printer_args
{
    const pfstest_nv_ptr char *message;
};

static void message_printer(pfstest_reporter_t *reporter, const void *object)
{
    const struct message_printer_args *args = object;
    pfstest_reporter_print_nv_string(reporter, args->message);
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

void _pfstest_suite_register_test(pfstest_list_t *suite,
                                  pfstest_t *the_test)
{
    pfstest_list_node_init((pfstest_list_node_t *)the_test);
    pfstest_list_append(suite, (pfstest_list_node_t *)the_test);
}

pfstest_list_t *pfstest_suite_get_tests(void)
{
    return &tests;
}

static void call_protected(void (*function)(void))
{
    if (0 == setjmp(dynamic_env->test_jmp_buf)) {
        function();
    }
}

static bool nv_strs_eq(
    const pfstest_nv_ptr char *s1, const pfstest_nv_ptr char *s2)
{

    return (0 == pfstest_strcmp_nvnv(s1, s2));
}

static bool str_eq_nv_str(
    const char *s1, const pfstest_nv_ptr char *s2)
{

    return (0 == pfstest_strcmp_nv(s1, s2));
}

static bool test_matches_filter(_pfstest_test_nv_t *test_desc,
                                const char *filter_file,
                                const char *filter_name)
{
    const pfstest_nv_ptr char *basename = pfstest_basename(test_desc->file);
    bool file_passed = (filter_file == NULL
                        || str_eq_nv_str(filter_file, basename));
    bool name_passed = (filter_name == NULL
                        || str_eq_nv_str(filter_name, test_desc->name));

    return name_passed && file_passed;
}

void pfstest_run(pfstest_t *the_test,
                 pfstest_list_t *before, pfstest_list_t *after,
                 const char *filter_file, const char *filter_name,
                 pfstest_reporter_t *reporter)
{
    dynamic_env_t local_dynamic_env;
    _pfstest_test_nv_t current_test;
    pfstest_list_node_t *hook_node;
    _pfstest_hook_nv_t current_hook;

    local_dynamic_env.reporter = reporter;
    dynamic_env_push(&local_dynamic_env);

    pfstest_memcpy_nv(&current_test, the_test->nv_data, sizeof(current_test));

    if (!test_matches_filter(&current_test, filter_file, filter_name))
        goto cleanup;

    pfstest_reporter_test_started(reporter,
                                  current_test.name, current_test.file);

    if (current_test.flags & _PFSTEST_FLAG_IGNORED) {
        pfstest_reporter_test_ignored(reporter);
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
        
                if (nv_strs_eq(current_test.file, current_hook.file))
                    current_hook.function();
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

            if (nv_strs_eq(current_test.file, current_hook.file))
            {
                /* Calling setjmp directly here was causing -Wclobber
                 * to issue a warning about after_hook. I think the
                 * warning was spurious, but I want to make the
                 * compiler happy. */
                call_protected(current_hook.function);
            }
        }
    }

    pfstest_alloc_free_frame();
    pfstest_alloc_frame_pop();

finish:
    pfstest_reporter_test_complete(reporter);

cleanup:
    dynamic_env_pop();
}

int pfstest_suite_run(pfstest_list_t *before, pfstest_list_t *after,
                      pfstest_list_t *suite,
                      const char *filter_file,
                      const char *filter_name,
                      pfstest_reporter_t *reporter)
{
    pfstest_list_node_t *test_node;
    int result;

    pfstest_reporter_run_started(reporter);

    pfstest_list_iter (test_node, suite) {
        pfstest_t *the_test = (pfstest_t *)test_node;

        pfstest_run(the_test, before, after,
                    filter_file, filter_name, reporter);
    }

    pfstest_reporter_run_complete(reporter);

    result = pfstest_reporter_return_value(reporter);

    return result;
}

void _pfstest_hook_list_register_hook(pfstest_list_t *list,
                                      pfstest_hook_t *hook)
{
    pfstest_list_node_init((pfstest_list_node_t *)hook);
    pfstest_list_append(list, (pfstest_list_node_t *)hook);
}

pfstest_list_t *pfstest_suite_get_before_hooks(void)
{
    return &before;
}

pfstest_list_t *pfstest_suite_get_after_hooks(void)
{
    return &after;
}

int pfstest_run_registered_tests(char *filter_file, char *filter_name,
                                 pfstest_reporter_t *reporter)
{
    return  pfstest_suite_run(&before, &after, &tests,
                              filter_file, filter_name,
                              reporter);
}
