.PHONY: run-test-targets
run-test-targets: $(call targets-exec-names,$(TARGETS))
	@set -e; \
	for runner in $^; \
	do \
	    echo; \
	    echo ./$${runner} $(ARGS); \
	    MALLOC_CHECK_=1 ./$${runner} $(ARGS); \
	done
