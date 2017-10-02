# -*- Makefile -*-

PFSTEST_SRC := $(addprefix src/, \
                 pfstest-basename.c pfstest-list.c pfstest-value.c \
                 pfstest-matcher.c pfstest-values.c pfstest-matchers.c \
                 pfstest-assert.c pfstest-core.c pfstest-arg-handler.c \
                 pfstest-arg-handlers.c pfstest-mock.c pfstest-output.c \
                 pfstest-equality.c pfstest-output-formatter-xml.c)

PFSTEST_STDINT_SRC := $(addprefix src/, \
                      pfstest-stdint-values.c pfstest-stdint-matchers.c)
