#ifndef PFSTEST_CORE_H
#define PFSTEST_CORE_H

#include <stdlib.h>

#include "pfstest-platform.h"
#include "pfstest-list.h"
#include "pfstest-reporter.h"

/* Basic utilities */

#define _pfstest_concat(a, b) a ## b
#define _pfstest_econcat(a, b) _pfstest_concat(a, b)

#define _pfstest_expect_semicolon struct _pfstest_expect_semicolon_stuct

/* So that they can be overridden in core tests */
#ifndef __PFSTEST_FILE__
#define __PFSTEST_FILE__ __FILE__
#endif
#ifndef __PFTEST_LINE__
#define __PFSTEST_LINE__ __LINE__
#endif

/* Fixtures */

typedef void (*_pfstest_fixture_hookp)(void);

#define _pfstest_fixture_hookp_name(name)               \
    _pfstest_econcat(__pfstest_fixture_hookp__, name)

#define _pfstest_fixture_hookp_decl(name)           \
    static const pfstest_pg _pfstest_fixture_hookp  \
    _pfstest_fixture_hookp_name(name)               \

#define _pfstest_fixture_hook_name(name)        \
    _pfstest_econcat(__pfstest_fixture__, name)

#define _pfstest_fixture_hook_decl(name)                \
    static void _pfstest_fixture_hook_name(name)(void)

#define _pfstest_fixture_hook_define(name)                              \
    _pfstest_fixture_hook_decl(name);                                   \
    _pfstest_fixture_hookp_decl(name) = _pfstest_fixture_hook_name(name); \
    _pfstest_fixture_hook_decl(name)

#define pfstest_setup() _pfstest_fixture_hook_define(setup_hook)
#define pfstest_teardown() _pfstest_fixture_hook_define(teardown_hook)

/* Tests */

#define _PFSTEST_FLAG_IGNORED 0x1

    typedef struct
    {
        const pfstest_pg_ptr char *name;
        const pfstest_pg_ptr char *file;
        int line;
        unsigned int flags;
        const pfstest_pg_ptr _pfstest_fixture_hookp *setup_function;
        const pfstest_pg_ptr _pfstest_fixture_hookp *teardown_function;
        void (*test_function)(void);
    } _pfstest_test_pg_t;

typedef struct
{
    pfstest_list_node_t list_node;
    const pfstest_pg_ptr _pfstest_test_pg_t *pg_data;
} pfstest_t;

#define _pfstest_function_name(name) _pfstest_econcat(__pfstest__, name)
#define _pfstest_name_var(name) _pfstest_econcat(__pfstest_name__, name)
#define _pfstest_file_var(name) _pfstest_econcat(__pfstest_file__, name)
#define _pfstest_pg_var(name) _pfstest_econcat(__pfstest_pg__, name)
#define _pfstest_func_decl(name)                    \
    static void _pfstest_function_name(name)(void)

#define _pfstest_object(name, flags, setup_ptr, teardown_ptr)           \
    static const pfstest_pg char _pfstest_name_var(name)[] = #name;     \
    static const pfstest_pg char _pfstest_file_var(name)[] =            \
        __PFSTEST_FILE__;                                               \
    static const pfstest_pg _pfstest_test_pg_t _pfstest_pg_var(name) =  \
    {                                                                   \
        _pfstest_name_var(name),                                        \
        _pfstest_file_var(name),                                        \
        __PFSTEST_LINE__, flags,                                        \
        setup_ptr, teardown_ptr,                                        \
        _pfstest_function_name(name),                                   \
    };                                                                  \
    pfstest_t name[1] = {{ {NULL}, &_pfstest_pg_var(name)}}

#define _pfstest_case_decl(name) extern pfstest_t name[]
#if defined(pfstest_constructor)
# define _pfstest_init_define(name)                                 \
    _pfstest_case_decl(the_test);                                   \
    pfstest_constructor(_pfstest_econcat(__pfstest_init__, name))   \
    {                                                               \
        pfstest_register_test(name);                                \
    }
#else  /* !defined(pfstest_constructor) */
# define _pfstest_init_define(name)
#endif

#define _pfstest_case_define(name, flags, setup_ptr, teardown_ptr)  \
    _pfstest_func_decl(name);                                       \
    _pfstest_object(name, flags, setup_ptr, teardown_ptr);          \
    _pfstest_func_decl(name)

#define _pfstest_case_without_fixture_define(name, flags)   \
    _pfstest_case_define(name, flags, NULL, NULL)

#define _pfstest_case_with_fixture_define(name, flags)                  \
    _pfstest_fixture_hookp_decl(setup_hook);                            \
    _pfstest_fixture_hookp_decl(teardown_hook);                         \
    _pfstest_case_define(name, flags,                                   \
                         &_pfstest_fixture_hookp_name(setup_hook),      \
                         &_pfstest_fixture_hookp_name(teardown_hook))

#define pfstest_case(name) _pfstest_case_without_fixture_define(name, 0)
#define pfstest_case_ignored(name)                                      \
    _pfstest_case_without_fixture_define(name, _PFSTEST_FLAG_IGNORED)
#define pfstest_case_with_fixture_ignored(name)                     \
    _pfstest_case_with_fixture_define(name, _PFSTEST_FLAG_IGNORED)

#define pfstest_case_with_fixture(name)         \
    _pfstest_case_with_fixture_define(name, 0)

void _pfstest_suite_register_test(pfstest_list_t *suite,
                                  pfstest_t *the_test);
#define pfstest_suite_register_test(suite, the_test) do {   \
        _pfstest_case_decl(the_test);                       \
        _pfstest_suite_register_test(suite, the_test);      \
    } while (0)

int pfstest_suite_run(pfstest_list_t *plugins, pfstest_list_t *suite,
                      const char *filter_file,
                      const char *filter_name,
                      pfstest_reporter_t *reporter);

#define _pfstest_define(name, flags)                \
    _pfstest_init_define(name)                      \
    _pfstest_case_with_fixture_define(name, flags)

#define pfstest(name) _pfstest_define(name, 0)
#define pfstest_ignore_test(name)                   \
    _pfstest_define(name, _PFSTEST_FLAG_IGNORED)

void _pfstest_register_test(pfstest_t *the_test);
#define pfstest_register_test(the_test) do {    \
        _pfstest_case_decl(the_test);           \
        _pfstest_register_test(the_test);       \
    } while (0)
pfstest_list_t *pfstest_get_registered_tests(void);

/* Plugins */

#define _PFSTEST_PLUGIN_CALLBACK_SETUP    0
#define _PFSTEST_PLUGIN_CALLBACK_CHECKS   1
#define _PFSTEST_PLUGIN_CALLBACK_TEARDOWN 2

typedef struct
{
    const pfstest_pg_ptr char *name;
    void (*callbacks[3])(void);
} _pfstest_plugin_pg_t;

typedef struct
{
    pfstest_list_node_t list_node;
    const pfstest_pg_ptr _pfstest_plugin_pg_t *pg_data;
} pfstest_plugin_t;

#define _pfstest_plugin_name_var(name)              \
    _pfstest_econcat(__pfstest_plugin_name__, name)

#define _pfstest_plugin_pg_var(name)                \
    _pfstest_econcat(__pfstest_plugin_pg__, name)

#define _pfstest_plugin_decl(plugin_name)       \
    extern pfstest_plugin_t plugin_name[]

#define pfstest_plugin_define(plugin_name, setup, checks, teardown)     \
    static const pfstest_pg char                                        \
    _pfstest_plugin_name_var(plugin_name)[] = #plugin_name;             \
    static const pfstest_pg _pfstest_plugin_pg_t                        \
    _pfstest_plugin_pg_var(plugin_name) =                               \
    {_pfstest_plugin_name_var(plugin_name), {setup, checks, teardown}}; \
    pfstest_plugin_t plugin_name[1] =                                   \
    {{ {NULL}, &_pfstest_plugin_pg_var(plugin_name) }}

#if defined(pfstest_constructor)

# define pfstest_plugin_autoload(name)                                  \
    pfstest_constructor(_pfstest_econcat(__pfstest_plugin_init__, name)) \
    {                                                                   \
        pfstest_register_plugin(name);                                  \
    } _pfstest_expect_semicolon

#else  /* !defined(pfstest_constructor) */
# define pfstest_plugin_autoload(name) _pfstest_plugin_decl(name)
#endif

void _pfstest_plugin_list_register_plugin(pfstest_list_t *plugins,
                                          pfstest_plugin_t *plugin);
#define pfstest_plugin_list_register_plugin(plugin_list, plugin_name)   \
    do {                                                                \
        _pfstest_plugin_decl(plugin_name);                              \
        _pfstest_plugin_list_register_plugin(plugin_list, plugin_name); \
    } while (0)

void _pfstest_register_plugin(pfstest_plugin_t *plugin);
#define pfstest_register_plugin(plugin_name)    \
    do {                                        \
        _pfstest_plugin_decl(plugin_name);      \
        _pfstest_register_plugin(plugin_name);  \
    } while (0)

pfstest_list_t *pfstest_get_registered_plugins(void);

/* Fail interface */

PFSTEST_NORETURN
void pfstest_fail_with_printer(
    const pfstest_pg_ptr char *file, int line,
    void (*printer)(pfstest_reporter_t *reporter, const void *),
    const void *object);

#define __PFSTEST_NV_FILE__ pfstest_pg_str(__PFSTEST_FILE__)

PFSTEST_NORETURN
void _pfstest_fail_at_location(
    const pfstest_pg_ptr char *file, int line,
    const pfstest_pg_ptr char *message);
#define pfstest_fail_at_location(file, line, message)               \
    _pfstest_fail_at_location(file, line, pfstest_pg_str(message))
#define pfstest_fail(message)                                           \
    pfstest_fail_at_location(__PFSTEST_NV_FILE__, __PFSTEST_LINE__, message)

/* Framework entry points */

int pfstest_run_registered_tests(char *filter_file, char *filter_name,
                                 pfstest_reporter_t *reporter);

/* Convenience aliases without the pfstest namespace prefix */

#ifndef PFSTEST_NOALIAS_test
# define test pfstest
#endif
#ifndef PFSTEST_NOALIAS_ignore_test
# define ignore_test pfstest_ignore_test
#endif
#ifndef PFSTEST_NOALIAS_fail
# define fail pfstest_fail
#endif
#ifndef PFSTEST_NOALIAS_register_test
# define register_test pfstest_register_test
#endif
#ifndef PFSTEST_NOALIAS_setup
# define setup pfstest_setup
#endif
#ifndef PFSTEST_NOALIAS_teardown
# define teardown pfstest_teardown
#endif
#ifndef PFSTEST_NOALIAS_register_plugin
# define register_plugin pfstest_register_plugin
#endif

#endif /* !PFSTEST_CORE_H */
