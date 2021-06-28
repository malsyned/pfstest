/* SPDX-License-Identifier: LGPL-2.1-or-later */

#ifndef PFSTEST_REPORTERS_STANDARD_H
#define PFSTEST_REPORTERS_STANDARD_H

/** @file */

#include "pfstest-reporter.h"

typedef enum {
    REPORT_COLOR_GREEN,
    REPORT_COLOR_YELLOW,
    REPORT_COLOR_RED,
    REPORT_COLOR_BOLD,
    REPORT_COLOR_VERY_RED
} pfstest_report_color_t;

struct _pfstest_report_colorizer_t
{
    void (*start)(int (*char_writer)(int), pfstest_report_color_t color);
    void (*reset)(int (*char_writer)(int));
};

typedef struct _pfstest_report_colorizer_t pfstest_report_colorizer_t;
/**
 * When passed to pfstest_reporter_standard_new() or
 * pfstest_reporter_verbose_new(), instructs the reporter not to
 * attempt to colorize the results. */
extern const pfstest_pg
pfstest_report_colorizer_t pfstest_report_colorizer_null[];
/**
 * When passed to pfstest_reporter_standard_new() or
 * pfstest_reporter_verbose_new(), instructs the reporter to use ANSI
 * color escapes to call attention to passes, failures, and ignored
 * tests with red/yellow/green indicators.
 */
extern const pfstest_pg
pfstest_report_colorizer_t pfstest_report_colorizer_ansi[];

/** Reports a "." for each successful test, "I" for each ignored test,
 * and a detailed failure description for each failing test. */
pfstest_reporter_t *pfstest_reporter_standard_new(
    int (*char_writer)(int),
    const pfstest_pg_ptr pfstest_report_colorizer_t *colorizer);
/** Reports file name, test name, result (PASS, IGNORED, FAIL), and a
 * detailed failure description for each failing test. */
pfstest_reporter_t *pfstest_reporter_verbose_new(
    int (*char_writer)(int),
    const pfstest_pg_ptr pfstest_report_colorizer_t *colorizer);

#endif /* !defined(PFSTEST_REPORTERS_STANDARD_H) */
