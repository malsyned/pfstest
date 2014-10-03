#include "pfstest-value.h"

void pfstest_value_print(pfstest_value_t *value)
{
    value->printer(value);
}
