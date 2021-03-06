# SPDX-License-Identifier: LGPL-2.1-or-later

include tests/selftestsrc.inc.mk
include util/automock-gcc.inc.mk

MAKEFLAGS += --no-builtin-rules --jobs

CC = $(CROSS_COMPILE)gcc
CFLAGS = -g -O3 $(WARN) -Wfatal-errors -std=c89
WARN = -Wall -Wextra -Werror -Wwrite-strings \
       -Wsuggest-attribute=noreturn -Wmissing-include-dirs \
       -Wswitch-default -Wfloat-equal -Wpointer-arith -Wundef \
       -Wbad-function-cast -Wcast-align \
       -Wjump-misses-init -Wconversion -Wlogical-op -Wstrict-prototypes \
       -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations \
       -Wvariadic-macros \
       -pedantic -pedantic-errors

AUTOMOCK_CPPFLAGS = $(GCC_AUTOMOCK_CPPFLAGS)
AUTOMOCK_FLAGS = $(GCC_AUTOMOCK_FLAGS)

BIN_PATTERN = %-runner
BUILDPREFIX = build/

CPPFLAGS = -Iinclude
SRC = $(wildcard src/core/*.c) \
      $(wildcard src/reporters/*.c) \
      $(wildcard src/matchers/*.c) \
      $(wildcard src/mock/*.c) \
      $(wildcard src/fp/*.c) \
      $(wildcard src/cli/*.c) \
      src/alloc/pfstest-alloc-malloc.c \
      src/main/gcc-main.c

TARGETS = selftest

selftest_SRC = $(SELFTEST_SRC) $(wildcard src/platform/avr8/tools/*.c)
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

all: doc
.PHONY: doc clean-doc
doc: examples
	doxygen
clean: clean-doc
clean-doc:
	$(if $(wildcard html),rm -rf html)

.PHONY: examples clean-examples
examples:
	$(MAKE) -C examples
clean: clean-examples
clean-examples:
	$(MAKE) -C examples clean

ifdef GCOV
CFLAGS += --coverage
LDFLAGS += --coverage

.PHONY: gcov
gcov:
	gcov -i $(call target-obj,$(TARGETS))

clean-files += $(call targets-files,$(TARGETS),.gcda)
clean-files += $(call targets-files,$(TARGETS),.gcno)
clean-files += $(addsuffix .gcov,$(notdir $(call targets-src,$(TARGETS))))
endif
