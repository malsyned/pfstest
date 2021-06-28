/* SPDX-License-Identifier: LGPL-2.1-or-later */

#ifndef PFSTEST_TAG_H
#define PFSTEST_TAG_H

typedef unsigned char pfstest_tag_t;

#define PFSTEST_TAG_AUTO 0

typedef struct
{
    pfstest_tag_t tag;
} pfstest_tagged_t;

void pfstest_tagged_init(pfstest_tagged_t *this, pfstest_tag_t *tag);
pfstest_tag_t pfstest_tag_get(void *this);

#endif /* !defined(PFSTEST_TAG_H) */
