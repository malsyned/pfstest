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
CPPFLAGS :=
LDLIBS :=

ALLOC_SRC := src/pfstest-alloc-malloc.c

SRC := $(COMMON_SRC) $(ALLOC_SRC) src/main/gcc-main.c

dep-flags := -MD -MP

src-dirs = $(sort $(dir $(SRC)))
auto-incs = $(addprefix -I,$(src-dirs))

all-cppflags = $(auto-incs) $(CPPFLAGS)

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

include Makefile.mock.in

%.o: %.c $(MAKEFILE_LIST)
	$(CC) $(dep-flags) $(CFLAGS) $(all-cppflags) -c -o $@ $<

%.i: %.c
	$(CC) $(all-cppflags) -E -o $@ $<

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
          $(SRC:%.c=%.o) $(SRC:%.c=%.d) $(SRC:%.c=%.i) \
          testrunner src/main/register-tests.c \
          tests/mock-dep.h tests/mock-dep.c

-include $(wildcard $(addsuffix *.d,$(dir $(SRC))))
