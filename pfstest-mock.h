#ifndef PFSTEST_MOCK_H
#define PFSTEST_MOCK_H

#include "pfstest-list.h"
#include "pfstest-value.h"
#include "pfstest-matcher.h"
#include "pfstest-arg-handler.h"

typedef struct 
{
    const char *name;
    const char *const *arg_names;
    int arg_count;
} pfstest_mock_t;

typedef struct 
{
    pfstest_list_node_t node;
    const pfstest_mock_t *mock;
    pfstest_arg_handler_t **arg_handlers;
    pfstest_value_t *return_value;
    int times;
} pfstest_expectation_t;

typedef struct
{
    pfstest_list_node_t node;
    pfstest_expectation_t *expectation;
} pfstest_invocation_t;

typedef struct _pfstest_verifier_t pfstest_verifier_t;

struct _pfstest_verifier_t
{
    pfstest_list_node_t node;
    void (*function)(pfstest_verifier_t *verifier);
    void *data;
};

typedef struct 
{
    pfstest_list_t expectations;
} pfstest_in_order_t;

void pfstest_mock_init(void);
pfstest_value_t *pfstest_mock_invoke(const pfstest_mock_t *mock,
                                     pfstest_value_t *default_return_value,
                                     ...);
pfstest_expectation_t *pfstest_when(const pfstest_mock_t *mock, ...);

pfstest_expectation_t *pfstest_do_return(pfstest_value_t *return_value,
                                         pfstest_expectation_t *expectation);
pfstest_expectation_t *pfstest_do_times(int times,
                                        pfstest_expectation_t *expectation);
pfstest_expectation_t *pfstest_one_time(pfstest_expectation_t *expectation);

void pfstest_verify_at_location(const char *file, int line,
                                pfstest_expectation_t *e);
#define pfstest_verify(e) pfstest_verify_at_location(__FILE__, __LINE__, e)

pfstest_in_order_t *pfstest_in_order_new(void);
void pfstest_in_order_verify_at_location(const char *file, int line,
                                         pfstest_in_order_t *order,
                                         pfstest_expectation_t *expectation);
#define pfstest_in_order_verify(order, expectation)         \
    pfstest_in_order_verify_at_location(__FILE__, __LINE__, \
                                        order, expectation)

void pfstest_run_verifiers(void);

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
