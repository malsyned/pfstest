#ifndef PFSTEST_H
#define PFSTEST_H

#include "pfstest-list.h"

#define _PFSTEST_FLAG_IGNORED 0x1
#define _PFSTEST_FLAG_EXPECT_FAIL 0x2

typedef struct
{
    pfstest_list_node_t list;
    const char *name;
    const char *file;
    unsigned int flags;
    void (*function)(void);
} pfstest_t;

#define _pfstest_function_name(name) __pfstest__ ## name
#define _pfstest_decl(name) static void _pfstest_function_name(name)(void)

#define _pfstest_object(name, flags)            \
    pfstest_t name[1] =                         \
    {{ {NULL}, # name, __FILE__, flags,         \
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
    _pfstest_init_define(name);                 \
    _pfstest_decl(name)

#define pfstest(name) _pfstest_protos(name, 0)
#define pfstest_failing_test(name)                      \
    _pfstest_protos(name, _PFSTEST_FLAG_EXPECT_FAIL)
#define pfstest_ignore_test(name)                   \
    _pfstest_protos(name, _PFSTEST_FLAG_IGNORED)

void pfstest_fail(const char *message);
void pfstest_fail_with_printer(void (*printer)(const void *),
                               const void *object);
void pfstest_register_test(pfstest_t *the_test);
int pfstest_run_tests(void);

typedef struct 
{
    pfstest_list_node_t list;
    void (*function)(void);
} pfstest_hook_t;

#define _pfstest_hook_name(name) __pfstest_hook__ ## name
#define _pfstest_hook_decl(name) static void _pfstest_hook_name(name)(void)

#define _pfstest_hook_object(name)              \
    pfstest_hook_t name[1] =                    \
    {{ {NULL}, _pfstest_hook_name(name) }}
         
#define _pfstest_hook_init_define(name, phase)      \
    __attribute__((__constructor__))                \
    static void __pfstest_hook_init__ ## name(void) \
    {                                               \
        pfstest_register_ ## phase(name);           \
    }

#define _pfstest_hook(name, phase)              \
    _pfstest_hook_decl(name);                   \
    _pfstest_hook_object(name);                 \
    _pfstest_hook_init_define(name, phase);     \
    _pfstest_hook_decl(name)

#define pfstest_before_tests(name) _pfstest_hook(name, before)
#define pfstest_after_tests(name) _pfstest_hook(name, after)

void pfstest_register_before(pfstest_hook_t *the_hook);
void pfstest_register_after(pfstest_hook_t *the_hook);

#ifndef PFSTEST_NOALIAS_TEST
# define test pfstest
#endif
#ifndef PFSTEST_NOALIAS_FAILING_TEST
# define failing_test pfstest_failing_test
#endif
#ifndef PFSTEST_NOALIAS_IGNORE_TEST
# define ignore_test pfstest_ignore_test
#endif
#ifndef PFSTEST_NOALIAS_FAIL
# define fail pfstest_fail
#endif
#ifndef PFSTEST_NOALIAS_REGISTER_TEST
# define register_test pfstest_register_test
#endif
#ifndef PFSTEST_NOALIAS_RUN_TESTS
# define run_tests pfstest_run_tests
#endif
#ifndef PFSTEST_NOALIAS_BEFORE_TESTS
# define before_tests pfstest_before_tests
#endif
#ifndef PFSTEST_NOALIAS_AFTER_TESTS
# define after_tests pfstest_after_tests
#endif
#ifndef PFSTEST_NOALIAS_REGISTER_BEFORE
# define register_before pfstest_register_before
#endif
#ifndef PFSTEST_NOALIAS_REGISTER_AFTER
# define register_after pfstest_register_after
#endif

#endif /* !PFSTEST_H */
