SELFTEST_SRC := $(addprefix tests/, \
                  pfstest-reporter-message-spy.c \
                  capture-output.c test-core.c core-test-cases.c \
                  core-test-cases-2.c \
                  test-assert-that.c test-values-and-matchers.c \
                  test-stdint.c test-mock.c test-xml.c test-color.c \
                  test-avr.c test-shorthand.c)

SELFTEST_CLI_SRC := tests/test-invocation.c

SELFTEST_MOCKS :=  tests/dep.h
