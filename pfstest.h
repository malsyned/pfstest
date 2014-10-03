#ifndef PFSTEST_H
#define PFSTEST_H

#include "list.h"

#define PFSTEST_FLAG_IGNORED 0x1
#define PFSTEST_FLAG_EXPECT_FAIL 0x2

struct pfstest
{
    struct list list;
    const char *name;
    const char *file;
    int line;
    unsigned int flags;
    void (*function)(void);
};

#define _test_function_name(name) __test__ ## name
#define _test_decl(name) static void _test_function_name(name)(void)

#define _test_struct(name, flags)                                       \
    struct pfstest name[1] =                                            \
    {{ {NULL}, # name, __FILE__, __LINE__, flags, _test_function_name(name) }}

#define _test_init_define(name)                 \
    __attribute__((__constructor__))            \
    static void __test_init__ ## name(void)     \
    {                                           \
        register_test(name);                    \
    }
    

#define _test_protos(name, flags)               \
    _test_decl(name);                           \
    _test_struct(name, flags);                  \
    _test_init_define(name);                    \
    _test_decl(name)

#define test(name) _test_protos(name, 0)
#define failing_test(name) _test_protos(name, PFSTEST_FLAG_EXPECT_FAIL)
#define ignore_test(name) _test_protos(name, PFSTEST_FLAG_IGNORED)

void fail(const char *message);
void register_test(struct pfstest *the_test);
int run_tests(void);

#endif /* !PFSTEST_H */
