#ifndef PFSTEST_CORE_H
#define PFSTEST_CORE_H

#include <stdlib.h>

#include "pfstest-platform.h"
#include "pfstest-list.h"
#include "pfstest-output.h"

/* Basic utilities */

#define _pfstest_concat(a, b) a ## b
#define _pfstest_econcat(a, b) _pfstest_concat(a, b)

/* Tests */

#define _PFSTEST_FLAG_IGNORED 0x1
#define _PFSTEST_FLAG_EXPECT_FAIL 0x2

typedef struct
{
    const pfstest_nv_ptr char *name;
    const pfstest_nv_ptr char *file;
    int line;
    unsigned int flags;
    void (*function)(void);
} _pfstest_test_nv_t;

typedef struct
{
    pfstest_list_node_t list_node;
    const pfstest_nv_ptr _pfstest_test_nv_t *nv_data;
} pfstest_t;

#define _pfstest_function_name(name) _pfstest_econcat(__pfstest__, name)
#define _pfstest_name_var(name) _pfstest_econcat(__pfstest_name__, name)
#define _pfstest_file_var(name) _pfstest_econcat(__pfstest_file__, name)
#define _pfstest_nv_var(name) _pfstest_econcat(__pfstest_nv__, name)
#define _pfstest_func_decl(name)                    \
    static void _pfstest_function_name(name)(void)

#define _pfstest_object_decl(name) pfstest_t name[]

#define _pfstest_object(name, flags)                                    \
    static const pfstest_nv char _pfstest_name_var(name)[] = #name;     \
    static const pfstest_nv char _pfstest_file_var(name)[] = __FILE__;  \
    static const pfstest_nv _pfstest_test_nv_t _pfstest_nv_var(name) =  \
    {                                                                   \
        _pfstest_name_var(name),                                        \
        _pfstest_file_var(name),                                        \
        __LINE__, flags,                                                \
        _pfstest_function_name(name),                                   \
    };                                                                  \
    pfstest_t name[1] = {{ {NULL}, &_pfstest_nv_var(name)}}

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

#define _pfstest_case_define(name, flags)       \
    _pfstest_func_decl(name);                   \
    _pfstest_object(name, flags);               \
    _pfstest_func_decl(name)

#define pfstest_case(name) _pfstest_case_define(name, 0)
#define pfstest_case_ignored(name)                      \
    _pfstest_case_define(name, _PFSTEST_FLAG_IGNORED)

void _pfstest_suite_register_test(pfstest_list_t *suite,
                                  pfstest_t *the_test);
#define pfstest_suite_register_test(suite, the_test) do {   \
        _pfstest_case_decl(the_test);                       \
        _pfstest_suite_register_test(suite, the_test);      \
    } while (0)

#define PFSTEST_SUITE_RUN_FLAGS_NONE 0
#define PFSTEST_SUITE_RUN_FLAG_VERBOSE 0x0001

int pfstest_suite_run(pfstest_list_t *before, pfstest_list_t *after,
                      pfstest_list_t *suite,
                      const pfstest_nv_ptr char *filter_file,
                      const pfstest_nv_ptr char *filter_name,
                      pfstest_output_formatter_t *formatter);

#define _pfstest_define(name, flags)            \
    _pfstest_init_define(name)                  \
    _pfstest_case_define(name, flags)

#define pfstest(name) _pfstest_define(name, 0)
#define pfstest_failing_test(name)                      \
    _pfstest_define(name, _PFSTEST_FLAG_EXPECT_FAIL)
#define pfstest_ignore_test(name)                   \
    _pfstest_define(name, _PFSTEST_FLAG_IGNORED)
#define pfstest_ignore_failing_test pfstest_ignore_test

void _pfstest_register_test(pfstest_t *the_test);
#define pfstest_register_test(the_test) do {    \
        _pfstest_case_decl(the_test);           \
        _pfstest_register_test(the_test);       \
    } while (0)

/* Hooks (before, after) */

typedef struct 
{
    const pfstest_nv_ptr char *file;
    const pfstest_nv_ptr char *name;
    void (*function)(void);
} _pfstest_hook_nv_t;

typedef struct 
{
    pfstest_list_node_t list_node;
    const pfstest_nv_ptr _pfstest_hook_nv_t *nv_data;
} pfstest_hook_t;

#define _pfstest_hook_name(name) _pfstest_econcat(__pfstest_hook__, name)
#define _pfstest_hook_nv_var(name)              \
    _pfstest_econcat(__pfstest_hook_nv__, name)
#define _pfstest_hook_file_var(name)                \
    _pfstest_econcat(__pfstest_hook_file__, name)
#define _pfstest_hook_name_var(name)                \
    _pfstest_econcat(__pfstest_hook_name__, name)
#define _pfstest_hook_func_decl(name)           \
    static void _pfstest_hook_name(name)(void)

#define _pfstest_hook_object(name)                                  \
    static const pfstest_nv char _pfstest_hook_file_var(name)[] =   \
        __FILE__;                                                   \
    static const pfstest_nv char _pfstest_hook_name_var(name)[] =   \
        #name;                                                      \
    static const pfstest_nv _pfstest_hook_nv_t                      \
    _pfstest_hook_nv_var(name) =                                    \
    {                                                               \
        _pfstest_hook_file_var(name),                               \
        _pfstest_hook_name_var(name),                               \
        _pfstest_hook_name(name)                                    \
    };                                                              \
    pfstest_hook_t name[1] =                                        \
    {{ {NULL}, &_pfstest_hook_nv_var(name) }}

#define _pfstest_hook_decl(name) extern pfstest_hook_t name[]

#if defined(pfstest_constructor)
#define _pfstest_hook_init_define(name, phase)                          \
    _pfstest_hook_decl(name);                                           \
    pfstest_constructor(_pfstest_econcat(__pfstest_hook_init__, name))  \
    {                                                                   \
        _pfstest_econcat(pfstest_register_, phase)(name);               \
    }
#else  /* !defined(pfstest_constructor) */
# define _pfstest_hook_init_define(name, phase)
#endif

#define pfstest_hook(name)                      \
    _pfstest_hook_func_decl(name);              \
    _pfstest_hook_object(name);                 \
    _pfstest_hook_func_decl(name)

#define _pfstest_hook(name, phase)              \
    _pfstest_hook_init_define(name, phase)      \
    pfstest_hook(name)

void _pfstest_hook_list_register_hook(pfstest_list_t *list,
                                      pfstest_hook_t *hook);
#define pfstest_hook_list_register_hook(list, hook) do {    \
        _pfstest_hook_decl(hook);                           \
        _pfstest_hook_list_register_hook(list, hook);       \
    } while (0)

#define pfstest_before_tests(name) _pfstest_hook(name, before)
#define pfstest_after_tests(name) _pfstest_hook(name, after)

#define _pfstest_register_hook(the_hook, phase) do {            \
        _pfstest_hook_decl(the_hook);                           \
        _pfstest_econcat(_pfstest_register_, phase)(the_hook);  \
    } while (0)

void _pfstest_register_before(pfstest_hook_t *the_hook);
#define pfstest_register_before(the_hook)       \
    _pfstest_register_hook(the_hook, before)

void _pfstest_register_after(pfstest_hook_t *the_hook);
#define pfstest_register_after(the_hook)        \
    _pfstest_register_hook(the_hook, after)

/* Fail interface */

PFSTEST_NORETURN
void pfstest_fail_with_printer(
    const pfstest_nv_ptr char *file, int line,
    void (*printer)(pfstest_output_formatter_t *formatter,
                    const void *),
    const void *object);

PFSTEST_NORETURN
void _pfstest_fail_at_location(
    const pfstest_nv_ptr char *file, int line,
    const pfstest_nv_ptr char *message);
#define pfstest_fail_at_location(file, line, message)                   \
    _pfstest_fail_at_location(file, line, pfstest_nv_string(message))
#define pfstest_fail(message)                                           \
    pfstest_fail_at_location(pfstest_nv_string(__FILE__), __LINE__, message)

/* Framework entry points */

int pfstest_run_tests(int argc, char *argv[]);
int pfstest_run_all_tests(void);
int pfstest_run_all_tests_verbose(void);

void pfstest_protect_call(void (*f)(void),
                          pfstest_output_formatter_t *formatter);
int run_all_tests_new(void);

/* Convenience aliases without the pfstest namespace prefix */

#ifndef PFSTEST_NOALIAS_test
# define test pfstest
#endif
#ifndef PFSTEST_NOALIAS_failing_test
# define failing_test pfstest_failing_test
#endif
#ifndef PFSTEST_NOALIAS_ignore_test
# define ignore_test pfstest_ignore_test
#endif
#ifndef PFSTEST_NOALIAS_ignore_failing_test
# define ignore_failing_test pfstest_ignore_failing_test
#endif
#ifndef PFSTEST_NOALIAS_fail
# define fail pfstest_fail
#endif
#ifndef PFSTEST_NOALIAS_register_test
# define register_test pfstest_register_test
#endif
#ifndef PFSTEST_NOALIAS_run_tests
# define run_tests pfstest_run_tests
#endif
#ifndef PFSTEST_NOALIAS_run_all_tests
# define run_all_tests pfstest_run_all_tests
#endif
#ifndef PFSTEST_NOALIAS_run_all_tests_verbose
# define run_all_tests_verbose pfstest_run_all_tests_verbose
#endif
#ifndef PFSTEST_NOALIAS_before_tests
# define before_tests pfstest_before_tests
#endif
#ifndef PFSTEST_NOALIAS_after_tests
# define after_tests pfstest_after_tests
#endif
#ifndef PFSTEST_NOALIAS_register_before
# define register_before pfstest_register_before
#endif
#ifndef PFSTEST_NOALIAS_register_after
# define register_after pfstest_register_after
#endif

#endif /* !PFSTEST_CORE_H */
