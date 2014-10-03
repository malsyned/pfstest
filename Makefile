CFLAGS := -g -O0 -Wall -Werror -MD -MP

.PHONY: all
all: sample-tests

.PHONY: run
run: sample-tests
	echo ; ./sample-tests

sample-tests: pfstest-list.o pfstest-value.o pfstest-matcher.o \
              pfstest-assert.o pfstest.o \
              sample-tests.o assert-that-tests.o main.o

.PHONY: clean
clean:
	rm -f *.o *.d sample-tests

-include $(wildcard *.d)
