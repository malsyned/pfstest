#ifndef PFSTEST_PLATFORM_VCXX_H
#define PFSTEST_PLATFORM_VCXX_H

# define PFSTEST_NORETURN __declspec(noreturn)

# pragma section(".CRT$XCU",read)
# define pfstest_constructor(name)                                  \
    static void name(void);                                         \
    static __declspec(allocate(".CRT$XCU"))                         \
    void (* _pfstest_econcat(__pfstest_ctor__, name))(void) = name; \
    static void name(void)

#endif /* !PFSTEST_PLATFORM_VCXX_H */
