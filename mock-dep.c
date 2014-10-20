#include "mock-dep.h"

#include <stddef.h>

#include "pfstest-platform.h"
#include "pfstest-values.h"

pfstest_mock_define(mock_dep_func1, "dep_func1", 1);

int dep_func1(int i)
{
    pfstest_value_t *return_value =
        pfstest_mock_invoke(mock_dep_func1, the_int(0), the_int(i));

    return *(int *)pfstest_value_data(return_value);
}

pfstest_mock_define(mock_dep_func2, "dep_func2", 2);

void dep_func2(int i, char *s)
{
    pfstest_value_t *return_value =
        pfstest_mock_invoke(mock_dep_func2, NULL, the_int(i), the_pointer(s));

    (void)return_value;
}
