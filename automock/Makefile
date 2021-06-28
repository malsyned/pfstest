# SPDX-License-Identifier: LGPL-2.1-or-later

.PHONY: test

PYTHON := python3

test:
	@echo
	PYTHONPATH=pycparser $(PYTHON) -m unittest discover -s tests $(ARGS)
