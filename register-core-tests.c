#include "pfstest.h"
#include "register-tests.h"

static void register_core_tests(void)
{
    register_before(before1);
    register_before(before2);
    register_after(after1);
    register_after(after2);

    register_test(should_pass);
    register_test(should_FAIL);
    register_test(should_be_ignored);
    register_test(should_expect_failure);
    register_test(should_FAIL_to_fail);
    register_test(should_call_befores_and_afters);
}

static void register_core_2_tests(void)
{
    register_test(should_only_call_before_tests_hooks_in_same_file);
    register_test(should_only_call_after_tests_hooks_in_same_file);
}

void register_tests(void)
{
    register_core_tests();
    register_core_2_tests();
}
