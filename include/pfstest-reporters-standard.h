#ifndef PFSTEST_REPORTERS_STANDARD_H
#define PFSTEST_REPORTERS_STANDARD_H

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
extern const pfstest_pg
pfstest_report_colorizer_t pfstest_report_colorizer_null[];
extern const pfstest_pg
pfstest_report_colorizer_t pfstest_report_colorizer_ansi[];

pfstest_reporter_t *pfstest_reporter_standard_new(
    int (*char_writer)(int),
    const pfstest_pg_ptr pfstest_report_colorizer_t *colorizer);
pfstest_reporter_t *pfstest_reporter_verbose_new(
    int (*char_writer)(int),
    const pfstest_pg_ptr pfstest_report_colorizer_t *colorizer);

#endif /* !defined(PFSTEST_REPORTERS_STANDARD_H) */
