include Makefile.src.in

CC = cc

OPT := g
WARN := -Wall -Wextra -Werror -Wwrite-strings \
        -Wsuggest-attribute=noreturn -Wmissing-include-dirs \
        -Wswitch-default -Wfloat-equal -Wpointer-arith -Wundef \
        -Wunused-macros -Wbad-function-cast -Wcast-qual -Wcast-align \
        -Wjump-misses-init -Wconversion -Wlogical-op -Wstrict-prototypes \
        -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations \
        -Wvariadic-macros -Wunsuffixed-float-constants \
        -pedantic -pedantic-errors
CFLAGS := -g -O$(OPT) $(WARN) -std=c89
LDFLAGS :=
CPPFLAGS := -Isrc -Itests
LDLIBS :=

dep-flags := -MD -MP

.PHONY: all
all: testrunner src/main/register-tests.c

.PHONY: test
test: testrunner
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

ALLOC_SRC := src/pfstest-alloc-malloc.c

SRC := $(COMMON_SRC) $(ALLOC_SRC) src/main/gcc-main.c

include Makefile.mock.in

%.o: %.c $(MAKEFILE_LIST)
	$(CC) $(dep-flags) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

%.i: %.c
	$(CC) $(CPPFLAGS) -E -o $@ $<

testrunner: $(SRC:%.c=%.o)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

src/main/register-tests.c: testrunner src/main/register-tests.c.header \
                           src/main/register-tests.c.footer
	cat src/main/register-tests.c.header > $@
	./$< -r >> $@
	cat src/main/register-tests.c.footer >> $@

.PHONY: clean
clean:
	rm -f \
          $(foreach dir,src src/main tests,$(addprefix $(dir)/,*.o *.d *.i)) \
          testrunner src/main/register-tests.c \
          tests/mock-dep.h tests/mock-dep.c

-include $(wildcard src/*.d src/main/*.d test/*.d)
