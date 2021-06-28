# SPDX-License-Identifier: LGPL-2.1-or-later

this-makefile := $(lastword $(MAKEFILE_LIST))

selftest-path := $(dir $(this-makefile))

SELFTEST_SRC := $(addprefix $(selftest-path),				\
                  pfstest-reporter-message-spy.c capture-output.c	\
                  test-core.c core-test-cases.c core-test-cases-2.c	\
                  test-assert-that.c test-values-and-matchers.c		\
                  test-mock.c test-xml.c test-color.c test-avr.c	\
                  test-shorthand.c test-invocation.c test-macros.c	\
                  test-fp-values-and-matchers.c				\
                  test-fp-values-and-matchers-printf.c)

SELFTEST_MOCKS :=  $(selftest-path)dep.h
