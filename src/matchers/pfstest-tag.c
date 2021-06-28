/* SPDX-License-Identifier: LGPL-2.1-or-later */

#include "pfstest-tag.h"

static pfstest_tag_t next_auto_tag = 1;

static void autoassign_tag_value_if_necessary(pfstest_tag_t *tag)
{
    if (*tag == PFSTEST_TAG_AUTO)
        *tag = next_auto_tag++;
}

void pfstest_tagged_init(pfstest_tagged_t *this, pfstest_tag_t *tag)
{
    autoassign_tag_value_if_necessary(tag);

    this->tag = *tag;
}

pfstest_tag_t pfstest_tag_get(void *this)
{
    pfstest_tagged_t *tagged = this;
    return tagged->tag;
}
