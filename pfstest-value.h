#ifndef PFSTEST_VALUE_H
#define PFSTEST_VALUE_H

typedef struct _pfstest_value_t pfstest_value_t;

struct _pfstest_value_t
{
    void (*printer)(pfstest_value_t *);
    void *data;
};

void pfstest_value_print(pfstest_value_t *value);
pfstest_value_t *pfstest_value_new(void (*printer)(pfstest_value_t *),
                                   void *data);
void *pfstest_value_data(pfstest_value_t *value);

#endif /* !PFSTEST_VALUE_H */
