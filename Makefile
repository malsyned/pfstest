CFLAGS := -g -O0 -Wall -Werror -MD -MP

.PHONY: all
all: sample-tests

.PHONY: run
run: sample-tests
	echo ; ./sample-tests

sample-tests: list.o pfstest.o sample-tests.o main.o

.PHONY: clean
clean:
	rm -f *.o *.d sample-tests

-include $(wildcard *.d)
