#ifndef PFSTEST_MOCK_H
#define PFSTEST_MOCK_H

#include "pfstest-core.h"
#include "pfstest-list.h"
#include "pfstest-value.h"
#include "pfstest-matcher.h"
#include "pfstest-arg-handler.h"

/* Mock */

typedef struct 
{
    const pfstest_nv_ptr char *name;
    int arg_count;
} pfstest_mock_t;

#define _pfstest_mock_func_name_var(name)               \
    _pfstest_econcat(__pfstest_mock_func_name__, name)

#define pfstest_mock_define(mock_name, func_name, arg_count)    \
    static const pfstest_nv char                                \
    _pfstest_mock_func_name_var(mock_name)[] = func_name;       \
    const pfstest_nv pfstest_mock_t mock_name[1] = {{           \
            _pfstest_mock_func_name_var(mock_name), arg_count}}

#define pfstest_mock_declare(mock_name)                 \
    extern const pfstest_nv pfstest_mock_t mock_name[]

void pfstest_mock_init(void);
void pfstest_mock_finish(void);

/* Expectation */

typedef struct 
{
    pfstest_list_node_t node;
    const pfstest_nv_ptr pfstest_mock_t *mock;
    pfstest_arg_handler_t **arg_handlers;
    pfstest_value_t *return_value;
    int times;
} pfstest_expectation_t;

pfstest_expectation_t *pfstest_when(
    const pfstest_nv_ptr pfstest_mock_t *mock, ...);

pfstest_expectation_t *pfstest_do_return(pfstest_value_t *return_value,
                                         pfstest_expectation_t *expectation);
pfstest_expectation_t *pfstest_do_times(int times,
                                        pfstest_expectation_t *expectation);
pfstest_expectation_t *pfstest_one_time(pfstest_expectation_t *expectation);

/* Invocation */

typedef struct
{
    pfstest_list_node_t node;
    pfstest_expectation_t *expectation;
    bool mark;
} pfstest_invocation_t;

pfstest_value_t *pfstest_mock_invoke(
    const pfstest_nv_ptr pfstest_mock_t *mock,
    pfstest_value_t *default_return_value,
    ...);

/* Verification */

typedef struct _pfstest_verifier_t pfstest_verifier_t;
struct _pfstest_verifier_t
{
    pfstest_list_node_t node;
    void (*function)(pfstest_verifier_t *verifier);
    void *data;
};

void pfstest_run_verifiers(void);

/* Verification with mode */

typedef struct _pfstest_verify_mode_t pfstest_verify_mode_t;
struct _pfstest_verify_mode_t
{
    void (*function)(const pfstest_nv_ptr char *file, int line,
                     pfstest_verify_mode_t *mode,
                     pfstest_expectation_t *expectation);
    void *data;
};

void pfstest_verify_at_location(const pfstest_nv_ptr char *file, int line,
                                pfstest_expectation_t *e);
#define pfstest_verify(e)                                               \
    pfstest_verify_at_location(pfstest_nv_string(__FILE__), __LINE__, e)
void pfstest_verify_times_at_location(const pfstest_nv_ptr char *file,
                                      int line,
                                      pfstest_verify_mode_t *mode,
                                      pfstest_expectation_t *e);
#define pfstest_verify_times(m, e)                                  \
    pfstest_verify_times_at_location(pfstest_nv_string(__FILE__),   \
                                     __LINE__, m, e)
pfstest_verify_mode_t *pfstest_exactly(int times);
pfstest_verify_mode_t *pfstest_at_most(int times);
pfstest_verify_mode_t *pfstest_at_least(int times);

/* No more interactions verification */

void pfstest_verify_no_more_interactions_at_location(
    const pfstest_nv_ptr char *file,
    int line,
    const pfstest_nv_ptr pfstest_mock_t *mock);
#define pfstest_verify_no_more_interactions(m)          \
    pfstest_verify_no_more_interactions_at_location(    \
        pfstest_nv_string(__FILE__), __LINE__, m)

/* In order verification */

typedef struct 
{
    pfstest_list_t expectations;
} pfstest_in_order_t;

pfstest_in_order_t *pfstest_in_order_new(void);
void pfstest_in_order_verify_at_location(const pfstest_nv_ptr char *file,
                                         int line,
                                         pfstest_in_order_t *order,
                                         pfstest_expectation_t *expectation);
#define pfstest_in_order_verify(order, expectation)                     \
    pfstest_in_order_verify_at_location(pfstest_nv_string(__FILE__),    \
                                        __LINE__,                       \
                                        order, expectation)

/* Convenience aliases without the pfstest namespace prefix */

#ifndef PFSTEST_NOALIAS_when
# define when pfstest_when
#endif
#ifndef PFSTEST_NOALIAS_do_return
# define do_return pfstest_do_return
#endif
#ifndef PFSTEST_NOALIAS_do_times
# define do_times pfstest_do_times
#endif
#ifndef PFSTEST_NOALIAS_one_time
# define one_time pfstest_one_time
#endif
#ifndef PFSTEST_NOALIAS_verify
# define verify pfstest_verify
#endif
#ifndef PFSTEST_NOALIAS_verify_times
# define verify_times pfstest_verify_times
#endif
#ifndef PFSTEST_NOALIAS_exactly
# define exactly pfstest_exactly
#endif
#ifndef PFSTEST_NOALIAS_at_most
# define at_most pfstest_at_most
#endif
#ifndef PFSTEST_NOALIAS_at_least
# define at_least pfstest_at_least
#endif
#ifndef PFSTEST_NOALIAS_pfstest_verify_no_more_interactions
# define verify_no_more_interactions pfstest_verify_no_more_interactions
#endif
#ifndef PFSTEST_NOALIAS_in_order_t
# define in_order_t pfstest_in_order_t
#endif
#ifndef PFSTEST_NOALIAS_in_order_new
# define in_order_new pfstest_in_order_new
#endif
#ifndef PFSTEST_NOALIAS_in_order_verify
# define in_order_verify pfstest_in_order_verify
#endif

#endif /* !PFSTEST_MOCK_H */
