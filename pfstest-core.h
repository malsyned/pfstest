#ifndef PFSTEST_CORE_H
#define PFSTEST_CORE_H

#include <stdlib.h>

#include "pfstest-list.h"

#define _PFSTEST_FLAG_IGNORED 0x1
#define _PFSTEST_FLAG_EXPECT_FAIL 0x2

typedef struct
{
    pfstest_list_node_t list_node;
    const char *name;
    const char *file;
    int line;
    unsigned int flags;
    void (*function)(void);
} pfstest_t;

#define _pfstest_function_name(name) __pfstest__ ## name
#define _pfstest_decl(name) static void _pfstest_function_name(name)(void)

#define _pfstest_object(name, flags)                \
    pfstest_t name[1] =                             \
    {{ {NULL}, # name, __FILE__, __LINE__,flags,    \
       _pfstest_function_name(name) }}

#define _pfstest_init_define(name)              \
    __attribute__((__constructor__))            \
    static void __pfstest_init__ ## name(void)  \
    {                                           \
        pfstest_register_test(name);            \
    }

#define _pfstest_protos(name, flags)            \
    _pfstest_decl(name);                        \
    _pfstest_object(name, flags);               \
    _pfstest_init_define(name)                  \
    _pfstest_decl(name)

#define pfstest(name) _pfstest_protos(name, 0)
#define pfstest_failing_test(name)                      \
    _pfstest_protos(name, _PFSTEST_FLAG_EXPECT_FAIL)
#define pfstest_ignore_test(name)                   \
    _pfstest_protos(name, _PFSTEST_FLAG_IGNORED)
#define pfstest_ignore_failing_test pfstest_ignore_test

void pfstest_fail_at_location(const char *file, int line,
                              const char *message)
    __attribute__((__noreturn__));
void pfstest_fail_with_printer(const char *file, int line,
                               void (*printer)(const void *),
                               const void *object)
    __attribute__((__noreturn__));
void pfstest_register_test(pfstest_t *the_test);
int pfstest_run_tests(int argc, char *argv[]);
int pfstest_run_all_tests(void);
int pfstest_run_all_tests_verbose(void);
#define pfstest_fail(message)                               \
    pfstest_fail_at_location(__FILE__, __LINE__, message)

typedef struct 
{
    pfstest_list_node_t list_node;
    const char *file;
    void (*function)(void);
} pfstest_hook_t;

#define _pfstest_hook_name(name) __pfstest_hook__ ## name
#define _pfstest_hook_decl(name) static void _pfstest_hook_name(name)(void)

#define _pfstest_hook_object(name, file)            \
    pfstest_hook_t name[1] =                        \
    {{ {NULL}, file, _pfstest_hook_name(name) }}
         
#define _pfstest_hook_init_define(name, phase)      \
    __attribute__((__constructor__))                \
    static void __pfstest_hook_init__ ## name(void) \
    {                                               \
        pfstest_register_ ## phase(name);           \
    }

#define _pfstest_hook(name, file, phase)        \
    _pfstest_hook_decl(name);                   \
    _pfstest_hook_object(name, file);           \
    _pfstest_hook_init_define(name, phase)      \
    _pfstest_hook_decl(name)

#define pfstest_before_tests(name) _pfstest_hook(name, __FILE__, before)
#define pfstest_after_tests(name) _pfstest_hook(name, __FILE__, after)

void pfstest_register_before(pfstest_hook_t *the_hook);
void pfstest_register_after(pfstest_hook_t *the_hook);

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
