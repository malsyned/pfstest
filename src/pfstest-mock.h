#ifndef PFSTEST_MOCK_H
#define PFSTEST_MOCK_H

/** @file */

#include "pfstest-core.h"
#include "pfstest-list.h"
#include "pfstest-value.h"
#include "pfstest-matcher.h"
#include "pfstest-arg-handler.h"

/* Mock */

typedef struct 
{
    const pfstest_pg_ptr char *name;
    unsigned int arg_count;
} pfstest_mock_t;

#define _pfstest_mock_func_name_var(name)               \
    _pfstest_econcat(__pfstest_mock_func_name__, name)

#define pfstest_mock_define(mock_name, func_name, arg_count)    \
    static const pfstest_pg char                                \
    _pfstest_mock_func_name_var(mock_name)[] = func_name;       \
    const pfstest_pg pfstest_mock_t mock_name[1] = {{           \
            _pfstest_mock_func_name_var(mock_name), arg_count}}

#define pfstest_mock_declare(mock_name)                 \
    extern const pfstest_pg pfstest_mock_t mock_name[]

void pfstest_mock_setup(void);
void pfstest_mock_teardown(void);

/* Expectation */

/** Describes an expected invocation of a mock and how it should be
 * handled */
typedef struct 
{
    pfstest_list_node_t node;
    const pfstest_pg_ptr pfstest_mock_t *mock;
    pfstest_arg_handler_t **arg_handlers;
    const pfstest_pg_ptr char *return_value_file;
    int return_value_line;
    pfstest_value_t *return_value;
    int times;
} pfstest_expectation_t;

/** Create a new expectation for a mock */
pfstest_expectation_t *pfstest_when(
    const pfstest_pg_ptr pfstest_mock_t *mock, ...);

pfstest_expectation_t *pfstest_do_return_at_location(
    const pfstest_pg_ptr char *file, int line,
    pfstest_value_t *return_value, pfstest_expectation_t *expectation);
/** Modify an expectation to return a value other than the default */
#define pfstest_do_return(return_value, expectation)                    \
    pfstest_do_return_at_location(__PFSTEST_NV_FILE__, __PFSTEST_LINE__, \
                                  return_value, expectation)

/** Modify an expectation to only match @p times times */
pfstest_expectation_t *pfstest_do_times(int times,
                                        pfstest_expectation_t *expectation);
/** Modify an expectation to only match once */
pfstest_expectation_t *pfstest_one_time(pfstest_expectation_t *expectation);

/* Invocation */

typedef struct
{
    pfstest_list_node_t node;
    pfstest_expectation_t *expectation;
    pfstest_bool mark;
} pfstest_invocation_t;

pfstest_value_t *pfstest_mock_invoke(
    const pfstest_pg_ptr pfstest_mock_t *mock,
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

void pfstest_mock_run_verifiers(void);

/* Verification with mode */

/**
 * Describes variations on how an invocation-counting verifier should
 * count invocations
 */
typedef struct _pfstest_verify_mode_t pfstest_verify_mode_t;
struct _pfstest_verify_mode_t
{
    void (*function)(const pfstest_pg_ptr char *file, int line,
                     pfstest_verify_mode_t *mode,
                     pfstest_expectation_t *expectation);
    void *data;
};

void pfstest_verify_at_location(const pfstest_pg_ptr char *file, int line,
                                pfstest_expectation_t *e);
/** Fail the current test if the expectation @p e never matches an
 * invocation */
#define pfstest_verify(e)                                               \
    pfstest_verify_at_location(__PFSTEST_NV_FILE__, __PFSTEST_LINE__, e)
void pfstest_verify_times_at_location(const pfstest_pg_ptr char *file,
                                      int line,
                                      pfstest_verify_mode_t *mode,
                                      pfstest_expectation_t *e);
/** Fail the current test if the expectation @p e never matches the
 * number of times described by @p m */
#define pfstest_verify_times(m, e)                              \
    pfstest_verify_times_at_location(__PFSTEST_NV_FILE__,       \
                                     __PFSTEST_LINE__, m, e)
/** As an argument to #pfstest_verify_times, require the expectation
 * to be met exactly @p times */
pfstest_verify_mode_t *pfstest_exactly(int times);
/** As an argument to #pfstest_verify_times, require the expectation
 * to be met at most @p times */
pfstest_verify_mode_t *pfstest_at_most(int times);
/** As an argument to #pfstest_verify_times, require the expectation
 * to be met at least @p times */
pfstest_verify_mode_t *pfstest_at_least(int times);

/* No more interactions verification */

void pfstest_verify_no_more_interactions_at_location(
    const pfstest_pg_ptr char *file,
    int line,
    const pfstest_pg_ptr pfstest_mock_t *mock);
/** Fail the current test if the function mocked by @p m has been
 * called additional times beyond what was expected */
#define pfstest_verify_no_more_interactions(m)          \
    pfstest_verify_no_more_interactions_at_location(    \
        __PFSTEST_NV_FILE__, __PFSTEST_LINE__, m)

/* No more invocations verification */

void pfstest_verify_no_more_invocations_at_location(
    const pfstest_pg_ptr char *file, int line);
/** Fail the current test if any mocked function has been called
 * additional times beyond what was expected */
#define pfstest_verify_no_more_invocations()        \
    pfstest_verify_no_more_invocations_at_location( \
        __PFSTEST_NV_FILE__, __PFSTEST_LINE__)

/* In order verification */

/** An in-order execution expectation context */
typedef struct 
{
    pfstest_list_t expectations;
} pfstest_in_order_t;

/** Create a new in-order verification context */
pfstest_in_order_t *pfstest_in_order_new(void);
void pfstest_in_order_verify_at_location(const pfstest_pg_ptr char *file,
                                         int line,
                                         pfstest_in_order_t *order,
                                         pfstest_expectation_t *expectation);
/** Add an expectation to be verified at the end of @p order */
#define pfstest_in_order_verify(order, expectation)             \
    pfstest_in_order_verify_at_location(__PFSTEST_NV_FILE__,    \
                                        __PFSTEST_LINE__,       \
                                        order, expectation)

/* Convenience aliases without the pfstest namespace prefix */

#ifndef PFSTEST_NOALIAS_when
/** @nonamespace_alias{PFSTEST_NOALIAS_when} */
# define when pfstest_when
#endif
#ifndef PFSTEST_NOALIAS_do_return
/** @nonamespace_alias{PFSTEST_NOALIAS_do_return} */
# define do_return pfstest_do_return
#endif
#ifndef PFSTEST_NOALIAS_do_times
/** @nonamespace_alias{PFSTEST_NOALIAS_do_times} */
# define do_times pfstest_do_times
#endif
#ifndef PFSTEST_NOALIAS_one_time
/** @nonamespace_alias{PFSTEST_NOALIAS_one_time} */
# define one_time pfstest_one_time
#endif
#ifndef PFSTEST_NOALIAS_verify
/** @nonamespace_alias{PFSTEST_NOALIAS_verify} */
# define verify pfstest_verify
#endif
#ifndef PFSTEST_NOALIAS_verify_times
/** @nonamespace_alias{PFSTEST_NOALIAS_verify_times} */
# define verify_times pfstest_verify_times
#endif
#ifndef PFSTEST_NOALIAS_exactly
/** @nonamespace_alias{PFSTEST_NOALIAS_exactly} */
# define exactly pfstest_exactly
#endif
#ifndef PFSTEST_NOALIAS_at_most
/** @nonamespace_alias{PFSTEST_NOALIAS_at_most} */
# define at_most pfstest_at_most
#endif
#ifndef PFSTEST_NOALIAS_at_least
/** @nonamespace_alias{PFSTEST_NOALIAS_at_least} */
# define at_least pfstest_at_least
#endif
#ifndef PFSTEST_NOALIAS_verify_no_more_interactions
/** @nonamespace_alias{PFSTEST_NOALIAS_verify_no_more_interactions} */
# define verify_no_more_interactions pfstest_verify_no_more_interactions
#endif
#ifndef PFSETEST_NOALIAS_verify_no_more_invocations
/** @nonamespace_alias{PFSETEST_NOALIAS_verify_no_more_invocations} */
#define verify_no_more_invocations pfstest_verify_no_more_invocations
#endif
#ifndef PFSTEST_NOALIAS_in_order_t
/** @nonamespace_alias{PFSTEST_NOALIAS_in_order_t} */
# define in_order_t pfstest_in_order_t
#endif
#ifndef PFSTEST_NOALIAS_in_order_new
/** @nonamespace_alias{PFSTEST_NOALIAS_in_order_new} */
# define in_order_new pfstest_in_order_new
#endif
#ifndef PFSTEST_NOALIAS_in_order_verify
/** @nonamespace_alias{PFSTEST_NOALIAS_in_order_verify} */
# define in_order_verify pfstest_in_order_verify
#endif

#endif /* !PFSTEST_MOCK_H */
