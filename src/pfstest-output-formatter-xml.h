#ifndef PFSTEST_OUTPUT_FORMATTER_XML_H
#define PFSTEST_OUTPUT_FORMATTER_XML_H

#include "pfstest-output.h"

pfstest_output_formatter_t *pfstest_output_formatter_xml_new(
    int (*print_char)(int));

#endif /* !PFSTEST_OUTPUT_FORMATTER_XML_H */
