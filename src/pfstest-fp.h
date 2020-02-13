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
pfstest_matcher_t *pfstest_is_a_float_within(float n, float tolerance);
/** Matches a float if @p n is within 1e-6 */
#define pfstest_is_a_float_near(n) pfstest_is_a_float_within(n, 1e-6f)

pfstest_matcher_t *_pfstest_is_float_inf(void);
/** Matches float positive infinity */
#define pfstest_is_float_inf _pfstest_is_float_inf()

pfstest_matcher_t *_pfstest_is_float_neg_inf(void);
/** Matches float negative infinity */
#define pfstest_is_float_neg_inf _pfstest_is_float_neg_inf()

pfstest_matcher_t *_pfstest_is_float_nan(void);
/** Matches IEEE-754 float Not-a-Number */
#define pfstest_is_float_nan _pfstest_is_float_nan()

/** Matches a double if @p n is within @p tolerance */
pfstest_matcher_t *pfstest_is_a_double_within(double n, double tolerance);
/** Matches a double if @p n is within 1e-6 */
#define pfstest_is_a_double_near(n) pfstest_is_a_double_within(n, 1e-12f)

pfstest_matcher_t *_pfstest_is_double_inf(void);
/** Matches double positive infinity */
#define pfstest_is_double_inf _pfstest_is_double_inf()

pfstest_matcher_t *_pfstest_is_double_neg_inf(void);
/** Matches double negative infinity */
#define pfstest_is_double_neg_inf _pfstest_is_double_neg_inf()

pfstest_matcher_t *_pfstest_is_double_nan(void);
/** Matches IEEE-754 double Not-a-Number */
#define pfstest_is_double_nan _pfstest_is_double_nan()

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
#ifndef PFSTEST_NOALIAS_is_a_float_within
/** @nonamespace_alias{PFSTEST_NOALIAS_is_a_float_within} */
# define is_a_float_within pfstest_is_a_float_within
#endif
#ifndef PFSTEST_NOALIAS_is_a_float_near
/** @nonamespace_alias{PFSTEST_NOALIAS_is_a_float_near} */
# define is_a_float_near pfstest_is_a_float_near
#endif
#ifndef PFSTEST_NOALIAS_is_float_inf
/** @nonamespace_alias{PFSTEST_NOALIAS_is_float_inf} */
# define is_float_inf pfstest_is_float_inf
#endif
#ifndef PFSTEST_NOALIAS_is_float_neg_inf
/** @nonamespace_alias{PFSTEST_NOALIAS_is_float_neg_inf} */
# define is_float_neg_inf pfstest_is_float_neg_inf
#endif
#ifndef PFSTEST_NOALIAS_is_float_nan
/** @nonamespace_alias{PFSTEST_NOALIAS_is_float_nan} */
# define is_float_nan pfstest_is_float_nan
#endif

#ifndef PFSTEST_NOALIAS_the_double
/** @nonamespace_alias{PFSTEST_NOALIAS_the_double} */
# define the_double pfstest_the_double
#endif
#ifndef PFSTEST_NOALIAS_is_a_double_within
/** @nonamespace_alias{PFSTEST_NOALIAS_is_a_double_within} */
# define is_a_double_within pfstest_is_a_double_within
#endif
#ifndef PFSTEST_NOALIAS_is_a_double_near
/** @nonamespace_alias{PFSTEST_NOALIAS_is_a_double_near} */
# define is_a_double_near pfstest_is_a_double_near
#endif
#ifndef PFSTEST_NOALIAS_is_double_inf
/** @nonamespace_alias{PFSTEST_NOALIAS_is_double_inf} */
# define is_double_inf pfstest_is_double_inf
#endif
#ifndef PFSTEST_NOALIAS_is_double_neg_inf
/** @nonamespace_alias{PFSTEST_NOALIAS_is_double_neg_inf} */
# define is_double_neg_inf pfstest_is_double_neg_inf
#endif
#ifndef PFSTEST_NOALIAS_is_double_nan
/** @nonamespace_alias{PFSTEST_NOALIAS_is_double_nan} */
# define is_double_nan pfstest_is_double_nan
#endif

/** @} */

#endif /* !PFSTEST_FP_H */
