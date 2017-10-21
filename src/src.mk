this-makefile := $(lastword $(MAKEFILE_LIST))

src-path := $(dir $(this-makefile))

PFSTEST_SRC := $(addprefix $(src-path), \
                 pfstest-list.c pfstest-value.c \
                 pfstest-matcher.c pfstest-values.c pfstest-matchers.c \
                 pfstest-assert.c pfstest-core.c pfstest-arg-handler.c \
                 pfstest-arg-handlers.c pfstest-mock.c pfstest-reporter.c \
                 pfstest-reporters-standard.c pfstest-equality.c \
                 pfstest-reporter-xml.c)

PFSTEST_AVR_TOOLS_SRC := $(src-path)avr/pfstest-avr-mem.c

PFSTEST_CLI_SRC := $(src-path)pfstest-cli.c

PFSTEST_STDINT_SRC := $(addprefix $(src-path), \
                      pfstest-stdint-values.c pfstest-stdint-matchers.c)
