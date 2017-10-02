MAKEFLAGS += --no-builtin-rules --jobs

CC = gcc
CFLAGS = -g -Og $(WARN) -std=c89
WARN = -Wall -Wextra -Werror -Wwrite-strings \
       -Wsuggest-attribute=noreturn -Wmissing-include-dirs \
       -Wswitch-default -Wfloat-equal -Wpointer-arith -Wundef \
       -Wunused-macros -Wbad-function-cast -Wcast-align \
       -Wjump-misses-init -Wconversion -Wlogical-op -Wstrict-prototypes \
       -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations \
       -Wvariadic-macros -Wunsuffixed-float-constants \
       -pedantic -pedantic-errors

EXEC_PATTERN = %-runner

BUILDPREFIX = build/
MOCKPREFIX = mocks/

include src/src.mk
include tests/selftestsrc.mk
SRC = $(PFSTEST_SRC) $(PFSTEST_STDINT_SRC) \
      src/pfstest-alloc-malloc.c src/main/gcc-main.c
selftest_SRC = $(SELFTEST_SRC) $(SELFTEST_MOCKS)

TARGETS = selftest

.DEFAULT_GOAL := all
.PHONY: all
all: targets src/main/register-tests.c

.PHONY: test
test: selftest-runner
	@echo ./$< $(ARGS); echo; \
	MALLOC_CHECK_=1 ./$< $(ARGS); \
	retval=$$?; \
	if [ $$retval -eq 0 ]; then \
		color=42; text=PASS; \
	else \
		color=41; text=FAIL; \
	fi; \
	echo "\033[30;2;$${color}m$${text}\033[00m"; \
	return $$retval

include mock.mk

src/main/register-tests.c: selftest-runner \
                           src/main/register-tests.c.header \
                           src/main/register-tests.c.footer
	./$< -r >> $@ \
	  | cat src/main/register-tests.c.header \
	    - \
	    src/main/register-tests.c.footer \
	  > $@

clean: clean-targets
	rm -rf $(addprefix $(basename $(SELFTEST_MOCKS)),.c .h)
	rm -rf src/main/register-tests.c
	rm -rf build mocks

include util/multiple-targets.mk
