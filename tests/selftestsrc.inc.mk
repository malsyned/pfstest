this-makefile := $(lastword $(MAKEFILE_LIST))

selftest-path := $(dir $(this-makefile))

SELFTEST_SRC := $(addprefix $(selftest-path), \
                  pfstest-reporter-message-spy.c \
                  capture-output.c test-core.c core-test-cases.c \
                  core-test-cases-2.c \
                  test-assert-that.c test-values-and-matchers.c \
                  test-mock.c test-xml.c test-color.c \
                  test-avr.c test-shorthand.c test-invocation.c \
                  test-macros.c)

SELFTEST_MOCKS :=  $(selftest-path)dep.h
