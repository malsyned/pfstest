#ifndef PFSTEST_REPORTERS_STANDARD_H
#define PFSTEST_REPORTERS_STANDARD_H

#include "pfstest-reporter.h"

typedef struct _pfstest_report_colorizer_t pfstest_report_colorizer_t;
extern pfstest_report_colorizer_t *pfstest_report_colorizer_null;
extern pfstest_report_colorizer_t *pfstest_report_colorizer_ansi;

pfstest_reporter_t *pfstest_reporter_standard_new(
    int (*print_char)(int), pfstest_report_colorizer_t *colorizer);
pfstest_reporter_t *pfstest_reporter_verbose_new(
    int (*print_char)(int), pfstest_report_colorizer_t *colorizer);

#endif /* !defined(PFSTEST_REPORTERS_STANDARD_H) */
