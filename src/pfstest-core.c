#include "pfstest-core.h"

#include <setjmp.h>
#include <string.h>

#include "pfstest-platform.h"
#include "pfstest-basename.h"
#include "pfstest-alloc.h"

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

static pfstest_list_t registered_tests = PFSTEST_LIST_EMPTY();
static pfstest_list_t registered_before_hooks = PFSTEST_LIST_EMPTY();
static pfstest_list_t registered_after_hooks = PFSTEST_LIST_EMPTY();
static pfstest_list_t registered_plugins = PFSTEST_LIST_EMPTY();

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
    _pfstest_suite_register_test(&registered_tests, the_test);
}

void _pfstest_register_before(pfstest_hook_t *the_hook)
{
    _pfstest_hook_list_register_hook(&registered_before_hooks, the_hook);
}

void _pfstest_register_after(pfstest_hook_t *the_hook)
{
    _pfstest_hook_list_register_hook(&registered_after_hooks, the_hook);
}

void _pfstest_suite_register_test(pfstest_list_t *suite,
                                  pfstest_t *the_test)
{
    pfstest_list_node_init((pfstest_list_node_t *)the_test);
    pfstest_list_append(suite, (pfstest_list_node_t *)the_test);
}

pfstest_list_t *pfstest_get_registered_tests(void)
{
    return &registered_tests;
}

static void call_protected(void (*function)(void))
{
    if (0 == setjmp(dynamic_env->test_jmp_buf))
        function();
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

static _pfstest_test_nv_t extract_test_descriptor(pfstest_t *the_test)
{
    _pfstest_test_nv_t test_desc;
    pfstest_memcpy_nv(&test_desc, the_test->nv_data, sizeof(test_desc));
    return test_desc;
}

static bool test_matches_filter(pfstest_t *the_test,
                                const char *filter_file,
                                const char *filter_name)
{
    _pfstest_test_nv_t test_desc = extract_test_descriptor(the_test);
    const pfstest_nv_ptr char *basename =  pfstest_basename(test_desc.file);

    bool file_passed = (filter_file == NULL
                        || str_eq_nv_str(filter_file, basename));
    bool name_passed = (filter_name == NULL
                        || str_eq_nv_str(filter_name, test_desc.name));

    return name_passed && file_passed;
}

static void run_before_hooks(pfstest_list_t *before,
                             const pfstest_nv_ptr char *file)
{
    pfstest_list_node_t *hook_node;
    _pfstest_hook_nv_t current_hook;

    if (before != NULL) {
        pfstest_list_iter(hook_node, before) {
            pfstest_hook_t *before_hook = (pfstest_hook_t *)hook_node;
            pfstest_memcpy_nv(&current_hook, before_hook->nv_data,
                              sizeof(current_hook));
        
            if (nv_strs_eq(file, current_hook.file))
                current_hook.function();
        }
    }
}

static void run_after_hooks(pfstest_list_t *after,
                            const pfstest_nv_ptr char *file)
{
    pfstest_list_node_t *hook_node;
    _pfstest_hook_nv_t current_hook;

    if (after != NULL) {
        pfstest_list_iter(hook_node, after) {
            pfstest_hook_t *after_hook = (pfstest_hook_t *)hook_node;
            pfstest_memcpy_nv(&current_hook, after_hook->nv_data,
                              sizeof(current_hook));

            if (nv_strs_eq(file, current_hook.file))
            {
                /* Calling setjmp directly here was causing -Wclobber
                 * to issue a warning about after_hook. I think the
                 * warning was spurious, but I want to make the
                 * compiler happy. */
                call_protected(current_hook.function);
            }
        }
    }
}

static bool test_ignored(_pfstest_test_nv_t *test_desc)
{
    return 0 != (test_desc->flags & _PFSTEST_FLAG_IGNORED);
}

static void plugins_run_callback(pfstest_list_t *plugins, int id)
{
    pfstest_list_node_t *plugin_node;
    _pfstest_plugin_nv_t current_plugin;
    void (*callback)(void);

    if (plugins != NULL) {
        pfstest_list_iter (plugin_node, plugins) {
            pfstest_plugin_t *plugin = (pfstest_plugin_t *)plugin_node;
            pfstest_memcpy_nv(&current_plugin, plugin->nv_data,
                              sizeof(current_plugin));

            callback = current_plugin.callbacks[id];
            if (callback != NULL)
                callback();
        }
    }
}

static void setup_plugins(pfstest_list_t *plugins)
{
    plugins_run_callback(plugins, _PFSTEST_PLUGIN_CALLBACK_SETUP);
}

static void run_plugin_checks(pfstest_list_t *plugins)
{
    plugins_run_callback(plugins, _PFSTEST_PLUGIN_CALLBACK_CHECKS);
}

static void teardown_plugins(pfstest_list_t *plugins)
{
    plugins_run_callback(plugins, _PFSTEST_PLUGIN_CALLBACK_TEARDOWN);
}

void pfstest_run(pfstest_t *the_test,
                 pfstest_list_t *before, pfstest_list_t *after,
                 pfstest_list_t *plugins,
                 pfstest_reporter_t *reporter)
{
    _pfstest_test_nv_t current_test = extract_test_descriptor(the_test);
    dynamic_env_t local_dynamic_env;
    (void)plugins;

    dynamic_env_push(&local_dynamic_env);
    dynamic_env->reporter = reporter;
    pfstest_alloc_frame_push();

    setup_plugins(plugins);

    pfstest_reporter_test_started(reporter,
                                  current_test.name, current_test.file);
    if (test_ignored(&current_test)) {
        pfstest_reporter_test_ignored(reporter);
    } else {
        if (0 == setjmp(dynamic_env->test_jmp_buf)) {
            run_before_hooks(before, current_test.file);
            current_test.function();
            run_plugin_checks(plugins);
        }
        run_after_hooks(after, current_test.file);
    }
    pfstest_reporter_test_complete(reporter);

    teardown_plugins(plugins);

    pfstest_alloc_frame_pop();
    dynamic_env_pop();
}

int pfstest_suite_run(pfstest_list_t *before, pfstest_list_t *after,
                      pfstest_list_t *plugins, pfstest_list_t *suite,
                      const char *filter_file,
                      const char *filter_name,
                      pfstest_reporter_t *reporter)
{
    pfstest_list_node_t *test_node;

    pfstest_reporter_run_started(reporter);

    pfstest_list_iter (test_node, suite) {
        pfstest_t *the_test = (pfstest_t *)test_node;

        if (test_matches_filter(the_test, filter_file, filter_name))
            pfstest_run(the_test, before, after, plugins, reporter);
    }

    pfstest_reporter_run_complete(reporter);

    return pfstest_reporter_return_value(reporter);
}

void _pfstest_hook_list_register_hook(pfstest_list_t *list,
                                      pfstest_hook_t *hook)
{
    pfstest_list_node_init((pfstest_list_node_t *)hook);
    pfstest_list_append(list, (pfstest_list_node_t *)hook);
}

pfstest_list_t *pfstest_get_registered_before_hooks(void)
{
    return &registered_before_hooks;
}

pfstest_list_t *pfstest_get_registered_after_hooks(void)
{
    return &registered_after_hooks;
}

void _pfstest_plugin_list_register_plugin(pfstest_list_t *plugin_list,
                                          pfstest_plugin_t *plugin)
{
    pfstest_list_node_init((pfstest_list_node_t *)plugin);
    pfstest_list_append(plugin_list, (pfstest_list_node_t *)plugin);
}

void _pfstest_register_plugin(pfstest_plugin_t *plugin)
{
    _pfstest_plugin_list_register_plugin(&registered_plugins, plugin);
}

pfstest_list_t *pfstest_get_registered_plugins(void)
{
    return &registered_plugins;
}

int pfstest_run_registered_tests(char *filter_file, char *filter_name,
                                 pfstest_reporter_t *reporter)
{
    return  pfstest_suite_run(&registered_before_hooks,
                              &registered_after_hooks,
                              &registered_plugins, &registered_tests,
                              filter_file, filter_name,
                              reporter);
}
