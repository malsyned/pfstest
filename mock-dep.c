#include "mock-dep.h"

#include <stddef.h>
#include <string.h>

#include "pfstest-platform.h"
#include "pfstest-values.h"

pfstest_mock_define(mock_dep_func1, "dep_func1", 1);
int dep_func1(int __pfstest_arg_0)
{
    int __pfstest_default_return = 0;

    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_dep_func1,
                            the_pointer(&__pfstest_default_return),
                            the_pointer(&__pfstest_arg_0));

    return *(int *)pfstest_value_data(__pfstest_return_value);
}

pfstest_mock_define(mock_dep_func2, "dep_func2", 2);
void dep_func2(int __pfstest_arg_0, char *__pfstest_arg_1)
{
    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_dep_func2,
                            NULL,
                            the_pointer(&__pfstest_arg_0),
                            the_pointer(__pfstest_arg_1));

    (void)__pfstest_return_value;
}

pfstest_mock_define(mock_dep_func3, "dep_func3", 0);
char *dep_func3(void)
{
    char * __pfstest_default_return = NULL;

    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_dep_func3,
                            the_pointer(__pfstest_default_return));

    return (char *)pfstest_value_data(__pfstest_return_value);
}
