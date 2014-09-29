CFLAGS := -g -O0 -Wall -Werror -MD -MP

.PHONY: all
all: sample-tests

.PHONY: run
run: sample-tests
	./sample-tests

sample-tests: pfstest.o sample-tests.o

.PHONY: clean
clean:
	rm -f *.o *.d test mock_tests

-include $(wildcard *.d)
