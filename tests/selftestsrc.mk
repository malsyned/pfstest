SELFTEST_SRC := $(addprefix tests/, \
                  pfstest-output-formatter-message-spy.c \
                  capture-output.c test-core.c core-test-cases.c \
                  core-test-cases-2.c test-invocation.c \
                  test-assert-that.c test-values-and-matchers.c \
                  test-mock.c test-xml.c)
SELFTEST_MOCKS :=  $(MOCKPREFIX)tests/dep-mock.c