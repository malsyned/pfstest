# SPDX-License-Identifier: LGPL-2.1-or-later

# Do not include this file directly. Instead, Add it to
# MULTITARGET_PLUGINS to cause it to be included at the right point in
# multitarget.inc.mk

.PHONY: test
test: run-test-targets

.PHONY: run-test-targets
run-test-targets: $(call targets-bin-names,$(TARGETS))
	@set -e; \
	for runner in $^; \
	do \
	    echo; \
	    echo ./$${runner} $(ARGS); \
	    MALLOC_CHECK_=1 ./$${runner} $(ARGS); \
	done
