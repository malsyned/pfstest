this-makefile := $(lastword $(MAKEFILE_LIST))

src-path := $(dir $(this-makefile))

PFSTEST_CORE_SRC := $(addprefix $(src-path), pfstest-list.c	\
                      pfstest-core.c pfstest-reporter.c		\
                      pfstest-reporters-standard.c)

PFSTEST_MALLOC_SRC := $(src-path)pfstest-alloc-malloc.c

PFSTEST_ASSERT_SRC := $(addprefix $(src-path),pfstest-tag.c	\
                        pfstest-value.c pfstest-matcher.c	\
                        pfstest-values.c pfstest-matchers.c	\
                        pfstest-assert.c)

PFSTEST_FP_SRC := $(addprefix $(src-path),pfstest-fp.c	\
                    pfstest-fp-printf.c)

PFSTEST_XML_SRC := $(src-path)pfstest-reporter-xml.c

PFSTEST_MOCK_SRC := $(addprefix $(src-path), pfstest-mock.c		\
                      pfstest-arg-handler.c pfstest-arg-handlers.c)

PFSTEST_SRC := $(PFSTEST_CORE_SRC) $(PFSTEST_ASSERT_SRC)	\
               $(PFSTEST_FP_SRC) $(PFSTEST_XML_SRC)		\
               $(PFSTEST_MOCK_SRC)

PFSTEST_AVR_TOOLS_SRC := $(src-path)avr/pfstest-avr-mem.c

PFSTEST_CLI_SRC := $(src-path)pfstest-cli.c
