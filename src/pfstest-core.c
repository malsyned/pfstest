#include "pfstest-core.h"

#include <setjmp.h>
#include <string.h>

#include "pfstest-platform.h"
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
    const pfstest_pg_ptr char *file, int line,
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
    const pfstest_pg_ptr char *message;
};

static void message_printer(pfstest_reporter_t *reporter, const void *object)
{
    const struct message_printer_args *args = object;
    pfstest_reporter_print_pg_str(reporter, args->message);
}

void _pfstest_fail_at_location(
    const pfstest_pg_ptr char *file, int line,
    const pfstest_pg_ptr char *message)
{
    struct message_printer_args args;

    args.message = message;

    pfstest_fail_with_printer(file, line, message_printer,
                              (const void *)&args);
}

void _pfstest_suite_register_test(pfstest_list_t *suite,
                                  pfstest_t *the_test)
{
    pfstest_list_node_init((pfstest_list_node_t *)the_test);
    pfstest_list_append(suite, (pfstest_list_node_t *)the_test);
}

void _pfstest_register_test(pfstest_t *the_test)
{
    _pfstest_suite_register_test(&registered_tests, the_test);
}

pfstest_list_t *pfstest_get_registered_tests(void)
{
    return &registered_tests;
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

static pfstest_bool str_eq_pg_str(
    const char *s1, const pfstest_pg_ptr char *s2)
{

    return (0 == pfstest_strcmp_pg(s1, s2));
}

static void extract_test_descriptor(pfstest_t *the_test,
                                    _pfstest_test_pg_t *test_desc)
{
    pfstest_memcpy_pg(test_desc, the_test->pg_data, sizeof(*test_desc));
}

static pfstest_bool test_matches_filter(_pfstest_test_pg_t *test_desc,
                                        const char *filter_file,
                                        const char *filter_name)
{
    pfstest_bool file_passed =
        (filter_file == NULL || str_eq_pg_str(filter_file, test_desc->file));
    pfstest_bool name_passed =
        (filter_name == NULL || str_eq_pg_str(filter_name, test_desc->name));

    return name_passed && file_passed;
}

static void run_indirect_function(const _pfstest_void_funcp *fp)
{
    _pfstest_void_funcp func;
    if (fp != NULL) {
        pfstest_memcpy_pg(&func, fp, sizeof(func));
        if (func != NULL)
            func();
    }
}

static void run_setup_function(_pfstest_test_pg_t *current_test)
{
    run_indirect_function(current_test->setup_function);
}

static void run_test_function(_pfstest_test_pg_t *current_test)
{
    current_test->test_function();
}

static void run_teardown_function(_pfstest_test_pg_t *current_test)
{
    run_indirect_function(current_test->teardown_function);
}

static void extract_plugin_descriptor(pfstest_plugin_t *the_plugin,
                                    _pfstest_plugin_pg_t *plugin_desc)
{
    pfstest_memcpy_pg(plugin_desc, the_plugin->pg_data,
                      sizeof(*plugin_desc));
}

static void plugins_run_callback(pfstest_list_t *plugins, int id)
{
    pfstest_plugin_t *plugin;
    _pfstest_plugin_pg_t plugin_desc;
    void (*callback)(void);

    if (plugins == NULL)
        return;

    pfstest_list_iter (plugin, plugins) {
        extract_plugin_descriptor(plugin, &plugin_desc);

        callback = plugin_desc.callbacks[id];
        if (callback != NULL)
            callback();
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

static pfstest_bool test_ignored(_pfstest_test_pg_t *test_desc)
{
    return 0 != (test_desc->flags & _PFSTEST_FLAG_IGNORED);
}

static void run_test(_pfstest_test_pg_t *current_test,
                     pfstest_list_t *plugins,
                     pfstest_reporter_t *reporter)
{
    dynamic_env_t local_dynamic_env;

    dynamic_env_push(&local_dynamic_env);
    pfstest_alloc_frame_push();

    dynamic_env->reporter = reporter;
    pfstest_reporter_test_started(reporter,
                                  current_test->name, current_test->file);

    setup_plugins(plugins);
    if (test_ignored(current_test)) {
        pfstest_reporter_test_ignored(reporter);
    } else {
        if (0 == setjmp(dynamic_env->test_jmp_buf)) {
            run_setup_function(current_test);
            run_test_function(current_test);
            run_plugin_checks(plugins);
        }
        if (0 == setjmp(dynamic_env->test_jmp_buf)) {
            run_teardown_function(current_test);
        }
    }
    teardown_plugins(plugins);

    pfstest_reporter_test_complete(reporter);

    pfstest_alloc_frame_pop();
    dynamic_env_pop();
}

int pfstest_suite_run(pfstest_list_t *plugins, pfstest_list_t *suite,
                      const char *filter_file,
                      const char *filter_name,
                      pfstest_reporter_t *reporter)
{
    pfstest_t *the_test;
    _pfstest_test_pg_t test_desc;

    pfstest_reporter_run_started(reporter);

    pfstest_list_iter (the_test, suite) {
        extract_test_descriptor(the_test, &test_desc);

        if (test_matches_filter(&test_desc, filter_file, filter_name))
            run_test(&test_desc, plugins, reporter);
    }

    pfstest_reporter_run_complete(reporter);

    return pfstest_reporter_return_value(reporter);
}

int pfstest_run_registered_tests(char *filter_file, char *filter_name,
                                 pfstest_reporter_t *reporter)
{
    return  pfstest_suite_run(&registered_plugins, &registered_tests,
                              filter_file, filter_name,
                              reporter);
}
