/* SPDX-License-Identifier: LGPL-2.1-or-later */

#ifndef PFSTEST_FP_H
#define PFSTEST_FP_H

/** @file */

#include "pfstest-value.h"
#include "pfstest-matcher.h"

/** @name Values */
/** @{ */

/** Box a float */
pfstest_value_t *pfstest_the_float(float n);
/** Box a double */
pfstest_value_t *pfstest_the_double(double n);

/** @} */

/** @name Matchers */
/** @{ */

/** Matches a float if @p n is within @p tolerance */
pfstest_matcher_t *pfstest_close_to_float(float n, float tolerance);
/** Matches a float if @p n is within 1e-6 */
#define pfstest_nearly_float(n) pfstest_close_to_float(n, 1e-6f)

/** Matches float positive infinity */
pfstest_matcher_t *pfstest_an_inf_float(void);

/** Matches float negative infinity */
pfstest_matcher_t *pfstest_a_neg_inf_float(void);

/** Matches IEEE-754 float Not-a-Number */
pfstest_matcher_t *pfstest_a_nan_float(void);

/** Matches a double if @p n is within @p tolerance */
pfstest_matcher_t *pfstest_close_to_double(double n, double tolerance);
/** Matches a double if @p n is within 1e-12 */
#define pfstest_nearly_double(n) pfstest_close_to_double(n, 1e-12)

/** Matches double positive infinity */
pfstest_matcher_t *pfstest_an_inf_double(void);

/** Matches double negative infinity */
pfstest_matcher_t *pfstest_a_neg_inf_double(void);

/** Matches IEEE-754 double Not-a-Number */
pfstest_matcher_t *pfstest_a_nan_double(void);

/** @} */

/** @name Formatting */
/** @{ **/

/** Assign a different function to format and print floating point
 * numbers.
 *
 * By default only their type, not their value, is printed. */
void pfstest_fp_set_printer(void (*printer)(double, pfstest_reporter_t *));

/** @} */

/** @nonamespace_section */
/** @{ */

#ifndef PFSTEST_NOALIAS_the_float
/** @nonamespace_alias{PFSTEST_NOALIAS_the_float} */
# define the_float pfstest_the_float
#endif
#ifndef PFSTEST_NOALIAS_close_to_float
/** @nonamespace_alias{PFSTEST_NOALIAS_close_to_float} */
# define close_to_float pfstest_close_to_float
#endif
#ifndef PFSTEST_NOALIAS_nearly_float
/** @nonamespace_alias{PFSTEST_NOALIAS_nearly_float} */
# define nearly_float pfstest_nearly_float
#endif
#ifndef PFSTEST_NOALIAS_an_inf_float
/** @nonamespace_alias{PFSTEST_NOALIAS_an_inf_float} */
# define an_inf_float pfstest_an_inf_float
#endif
#ifndef PFSTEST_NOALIAS_a_neg_inf_float
/** @nonamespace_alias{PFSTEST_NOALIAS_a_neg_inf_float} */
# define a_neg_inf_float pfstest_a_neg_inf_float
#endif
#ifndef PFSTEST_NOALIAS_a_nan_float
/** @nonamespace_alias{PFSTEST_NOALIAS_a_nan_float} */
# define a_nan_float pfstest_a_nan_float
#endif

#ifndef PFSTEST_NOALIAS_the_double
/** @nonamespace_alias{PFSTEST_NOALIAS_the_double} */
# define the_double pfstest_the_double
#endif
#ifndef PFSTEST_NOALIAS_close_to_double
/** @nonamespace_alias{PFSTEST_NOALIAS_close_to_double} */
# define close_to_double pfstest_close_to_double
#endif
#ifndef PFSTEST_NOALIAS_nearly_double
/** @nonamespace_alias{PFSTEST_NOALIAS_nearly_double} */
# define nearly_double pfstest_nearly_double
#endif
#ifndef PFSTEST_NOALIAS_an_inf_double
/** @nonamespace_alias{PFSTEST_NOALIAS_an_inf_double} */
# define an_inf_double pfstest_an_inf_double
#endif
#ifndef PFSTEST_NOALIAS_a_neg_inf_double
/** @nonamespace_alias{PFSTEST_NOALIAS_a_neg_inf_double} */
# define a_neg_inf_double pfstest_a_neg_inf_double
#endif
#ifndef PFSTEST_NOALIAS_a_nan_double
/** @nonamespace_alias{PFSTEST_NOALIAS_a_nan_double} */
# define a_nan_double pfstest_a_nan_double
#endif

/** @} */

#endif /* !PFSTEST_FP_H */
