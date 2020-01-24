include src/src.inc.mk
include tests/selftestsrc.inc.mk
include util/automock-gcc.inc.mk

MAKEFLAGS += --no-builtin-rules --jobs

CC = gcc
CFLAGS = -g -O3 $(WARN) -Wfatal-errors -std=c89
WARN = -Wall -Wextra -Werror -Wwrite-strings \
       -Wsuggest-attribute=noreturn -Wmissing-include-dirs \
       -Wswitch-default -Wfloat-equal -Wpointer-arith -Wundef \
       -Wbad-function-cast -Wcast-align \
       -Wjump-misses-init -Wconversion -Wlogical-op -Wstrict-prototypes \
       -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations \
       -Wvariadic-macros -Wunsuffixed-float-constants \
       -pedantic -pedantic-errors

AUTOMOCK_CPPFLAGS = $(GCC_AUTOMOCK_CPPFLAGS)
AUTOMOCK_FLAGS = $(GCC_AUTOMOCK_FLAGS)

BIN_PATTERN = %-runner
BUILDPREFIX = build/

SRC = $(PFSTEST_SRC) $(PFSTEST_CLI_SRC) \
      $(PFSTEST_AVR_TOOLS_SRC) \
      src/pfstest-alloc-malloc.c src/main/gcc-main.c

TARGETS = selftest

selftest_SRC = $(SELFTEST_SRC) $(SELFTEST_CLI_SRC)
selftest_MOCKS = $(SELFTEST_MOCKS)

ARGS=-c

MULTITARGET_PLUGINS = util/automock.inc.mk util/test-runners.inc.mk
include util/multitarget.inc.mk

clean-files += src/main/register-tests.c

all: src/main/register-tests.c

test: test-automock

.PHONY: test-automock
test-automock: | run-test-targets
	@$(MAKE) --no-print-directory -C automock test

src/main/register-tests.c: selftest-runner \
                           src/main/register-tests.c.header \
                           src/main/register-tests.c.footer
	./$< -r \
	  | cat src/main/register-tests.c.header \
	    - \
	    src/main/register-tests.c.footer \
	  > $@
