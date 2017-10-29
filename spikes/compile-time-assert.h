#define _pfstest_compile_time_assert_1(expr, line)                      \
    typedef char                                                        \
    _pfstest_econcat(__pfstest_compile_time_assert_, line)[1 - 2 *!(expr)]

#define pfstest_compile_time_assert(expr)           \
    _pfstest_compile_time_assert_1(expr, __LINE__)

