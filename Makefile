include src/src.mk
include tests/selftestsrc.mk
include util/automock.mk

MAKEFLAGS += --no-builtin-rules --jobs

CC = gcc
CFLAGS = -g -Og $(WARN) -Wfatal-errors -std=c89
WARN = -Wall -Wextra -Werror -Wwrite-strings \
       -Wsuggest-attribute=noreturn -Wmissing-include-dirs \
       -Wswitch-default -Wfloat-equal -Wpointer-arith -Wundef \
       -Wbad-function-cast -Wcast-align \
       -Wjump-misses-init -Wconversion -Wlogical-op -Wstrict-prototypes \
       -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations \
       -Wvariadic-macros -Wunsuffixed-float-constants \
       -pedantic -pedantic-errors

AUTOMOCK_CPPFLAGS = $(GCC_AUTOMOCK_CPPFLAGS)
AUTOMOCK_ARGS = $(GCC_AUTOMOCK_FLAGS)

EXEC_PATTERN = %-runner
BUILDPREFIX = build/

SRC = $(PFSTEST_SRC) $(PFSTEST_STDINT_SRC) $(PFSTEST_CLI_SRC) \
      $(PFSTEST_AVR_TOOLS_SRC) \
      src/pfstest-alloc-malloc.c src/main/gcc-main.c

TARGETS = selftest

selftest_SRC = $(SELFTEST_SRC) $(SELFTEST_CLI_SRC)
selftest_MOCKS = $(SELFTEST_MOCKS)

ARGS=-c

include util/test-targets.mk
include util/test-runners.mk

.DEFAULT_GOAL := all
.PHONY: all
all: targets src/main/register-tests.c

.PHONY: test
test: run-test-targets

src/main/register-tests.c: selftest-runner \
                           src/main/register-tests.c.header \
                           src/main/register-tests.c.footer
	./$< -r \
	  | cat src/main/register-tests.c.header \
	    - \
	    src/main/register-tests.c.footer \
	  > $@

clean: clean-targets
	rm -rf src/main/register-tests.c
	rm -rf build
