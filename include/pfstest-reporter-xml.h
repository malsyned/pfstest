/* SPDX-License-Identifier: LGPL-2.1-or-later */

#ifndef PFSTEST_REPORTER_XML_H
#define PFSTEST_REPORTER_XML_H

/** @file */

#include "pfstest-reporter.h"

/** Produce a machine-readable XML report of test results */
pfstest_reporter_t *pfstest_reporter_xml_new(int (*print_char)(int));

#endif /* !PFSTEST_REPORTER_XML_H */
