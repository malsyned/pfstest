#ifndef PFSTEST_FP_H
#define PFSTEST_FP_H

#include "pfstest-value.h"
#include "pfstest-matcher.h"

pfstest_value_t *pfstest_the_float(float n);
pfstest_value_t *pfstest_the_double(double n);

pfstest_matcher_t *pfstest_is_a_float_within(float n, float tolerance);
#define pfstest_is_a_float_near(n) pfstest_is_a_float_within(n, 1e-6f)

pfstest_matcher_t *_pfstest_is_float_inf(void);
#define pfstest_is_float_inf _pfstest_is_float_inf()

pfstest_matcher_t *_pfstest_is_float_neg_inf(void);
#define pfstest_is_float_neg_inf _pfstest_is_float_neg_inf()

pfstest_matcher_t *_pfstest_is_float_nan(void);
#define pfstest_is_float_nan _pfstest_is_float_nan()

pfstest_matcher_t *pfstest_is_a_double_within(double n, double tolerance);
#define pfstest_is_a_double_near(n) pfstest_is_a_double_within(n, 1e-12f)

pfstest_matcher_t *_pfstest_is_double_inf(void);
#define pfstest_is_double_inf _pfstest_is_double_inf()

pfstest_matcher_t *_pfstest_is_double_neg_inf(void);
#define pfstest_is_double_neg_inf _pfstest_is_double_neg_inf()

pfstest_matcher_t *_pfstest_is_double_nan(void);
#define pfstest_is_double_nan _pfstest_is_double_nan()

void pfstest_fp_set_printer(void (*printer)(double, pfstest_reporter_t *));

#ifndef PFSTEST_NOALIAS_the_float
# define the_float pfstest_the_float
#endif
#ifndef PFSTEST_NOALIAS_is_a_float_within
# define is_a_float_within pfstest_is_a_float_within
#endif
#ifndef PFSTEST_NOALIAS_is_a_float_near
# define is_a_float_near pfstest_is_a_float_near
#endif
#ifndef PFSTEST_NOALIAS_is_float_inf
# define is_float_inf pfstest_is_float_inf
#endif
#ifndef PFSTEST_NOALIAS_is_float_neg_inf
# define is_float_neg_inf pfstest_is_float_neg_inf
#endif
#ifndef PFSTEST_NOALIAS_is_float_nan
# define is_float_nan pfstest_is_float_nan
#endif

#ifndef PFSTEST_NOALIAS_the_double
# define the_double pfstest_the_double
#endif
#ifndef PFSTEST_NOALIAS_is_a_double_within
# define is_a_double_within pfstest_is_a_double_within
#endif
#ifndef PFSTEST_NOALIAS_is_a_double_near
# define is_a_double_near pfstest_is_a_double_near
#endif
#ifndef PFSTEST_NOALIAS_is_double_inf
# define is_double_inf pfstest_is_double_inf
#endif
#ifndef PFSTEST_NOALIAS_is_double_neg_inf
# define is_double_neg_inf pfstest_is_double_neg_inf
#endif
#ifndef PFSTEST_NOALIAS_is_double_nan
# define is_double_nan pfstest_is_double_nan
#endif

#endif /* !PFSTEST_FP_H */