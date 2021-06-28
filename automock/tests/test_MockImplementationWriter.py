# SPDX-License-Identifier: LGPL-2.1-or-later

from unittest import TestCase

from compat import StringIO
from compat import MagicMock

from automock import MockImplementationWriter
from automock import MockInfo, ReturnHint
from automock import ArgInfo, ArgHint

boilerplate_includes = """
#include <stddef.h>
#include <string.h>

#include "pfstest-platform.h"
#include "pfstest-values.h"

"""

class MockImplementationWriterTests(TestCase):
    def setUp(self):
        self.maxDiff = None
        self.cBuffer = StringIO()
        self.mockpaths = MagicMock()
        self.mockpaths.mockheaderrelpath = "mock-mockable.h"

    def test_shouldWriteImplementation(self):
        # Given
        mockgen = MagicMock()
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "func1",
                     prototype = "void func1(void)",
                     return_text = "void",
                     return_hint = ReturnHint.VOID,
                     args_info = [])
        ]
        mock_c_writer = MockImplementationWriter(self.mockpaths, mockgen)

        # When
        mock_c_writer.write_implementation(self.cBuffer)

        # Then
        self.assertEqual(
            self.cBuffer.getvalue(),
            '#include "mock-mockable.h"\n'
            + boilerplate_includes
            + """pfstest_mock_define(mock_func1, "func1", 0);
void func1(void)
{
    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func1,
                            NULL);

    (void)__pfstest_return_value;
}

"""
        )

    def test_shouldWriteMultipleFunctions(self):
        # Given
        mockgen = MagicMock()
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "func1",
                     prototype = "void func1(void)",
                     return_text = "void",
                     return_hint = ReturnHint.VOID,
                     args_info = []),
            MockInfo(mockname = "mock_func2",
                     funcname = "func2",
                     prototype = "void func2(void)",
                     return_text = "void",
                     return_hint = ReturnHint.VOID,
                     args_info = []),
        ]
        mock_c_writer = MockImplementationWriter(self.mockpaths, mockgen)

        # When
        mock_c_writer.write_implementation(self.cBuffer)

        # Then
        self.assertEqual(
            self.cBuffer.getvalue(),
            '#include "mock-mockable.h"\n'
            + boilerplate_includes
            + """pfstest_mock_define(mock_func1, "func1", 0);
void func1(void)
{
    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func1,
                            NULL);

    (void)__pfstest_return_value;
}

pfstest_mock_define(mock_func2, "func2", 0);
void func2(void)
{
    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func2,
                            NULL);

    (void)__pfstest_return_value;
}

"""
        )

    def test_shouldHandlePrimitiveReturnTypes(self):
        # Given
        mockgen = MagicMock()
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "func1",
                     prototype = "int func1(void)",
                     return_text = "int",
                     return_hint = ReturnHint.PRIMITIVE,
                     args_info = [])
        ]
        mock_c_writer = MockImplementationWriter(self.mockpaths, mockgen)

        # When
        mock_c_writer.write_implementation(self.cBuffer)

        # Then
        self.assertEqual(
            self.cBuffer.getvalue(),
            '#include "mock-mockable.h"\n'
            + boilerplate_includes
            + """pfstest_mock_define(mock_func1, "func1", 0);
int func1(void)
{
    int __pfstest_default_return = 0;

    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func1,
                            pfstest_the_memory(
                                &__pfstest_default_return,
                                sizeof(__pfstest_default_return)));

    return *(int *)pfstest_value_data(__pfstest_return_value);
}

"""
        )

    def test_shouldHandlePointerReturnTypes(self):
        # Given
        mockgen = MagicMock()
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "func1",
                     prototype = "char *func1(void)",
                     return_text = "char *",
                     return_hint = ReturnHint.POINTER,
                     args_info = [])
        ]
        mock_c_writer = MockImplementationWriter(self.mockpaths, mockgen)

        # When
        mock_c_writer.write_implementation(self.cBuffer)

        # Then
        self.assertEqual(
            self.cBuffer.getvalue(),
            '#include "mock-mockable.h"\n'
            + boilerplate_includes
            + """pfstest_mock_define(mock_func1, "func1", 0);
char *func1(void)
{
    char * __pfstest_default_return = NULL;

    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func1,
                            pfstest_the_pointer(__pfstest_default_return));

    return (char *)pfstest_value_data(__pfstest_return_value);
}

"""
        )

    def test_shouldHandleStructReturnTypes(self):
        # Given
        mockgen = MagicMock()
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "func1",
                     prototype = "struct foo func1(void)",
                     return_text = "struct foo",
                     return_hint = ReturnHint.BLOB,
                     args_info = [])
        ]
        mock_c_writer = MockImplementationWriter(self.mockpaths, mockgen)

        # When
        mock_c_writer.write_implementation(self.cBuffer)

        # Then
        self.assertEqual(
            self.cBuffer.getvalue(),
            '#include "mock-mockable.h"\n'
            + boilerplate_includes
            + """pfstest_mock_define(mock_func1, "func1", 0);
struct foo func1(void)
{
    struct foo __pfstest_default_return;
    memset(&__pfstest_default_return, 0, sizeof(__pfstest_default_return));

    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func1,
                            pfstest_the_memory(
                                &__pfstest_default_return,
                                sizeof(__pfstest_default_return)));

    return *(struct foo *)pfstest_value_data(__pfstest_return_value);
}

"""
        )

    def test_shouldWriteBlobParameter(self):
        # Given
        mockgen = MagicMock()
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "func1",
                     prototype = "void func1(int __pfstest_arg_0)",
                     return_text = "void",
                     return_hint = ReturnHint.VOID,
                     args_info = [ArgInfo('__pfstest_arg_0', ArgHint.BLOB)]
            )]
        mock_c_writer = MockImplementationWriter(self.mockpaths, mockgen)

        # When
        mock_c_writer.write_implementation(self.cBuffer)

        # Then
        self.assertEqual(
            self.cBuffer.getvalue(),
            '#include "mock-mockable.h"\n'
            + boilerplate_includes
            + """pfstest_mock_define(mock_func1, "func1", 1);
void func1(int __pfstest_arg_0)
{
    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func1,
                            NULL,
                            pfstest_the_memory(&__pfstest_arg_0,
                                               sizeof(__pfstest_arg_0)));

    (void)__pfstest_return_value;
}

"""
        )

    def test_shouldWritePointerParameter(self):
        # Given
        mockgen = MagicMock()
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "func1",
                     prototype = "void func1(char *__pfstest_arg_0)",
                     return_text = "void",
                     return_hint = ReturnHint.VOID,
                     args_info = [ArgInfo('__pfstest_arg_0',
                                          ArgHint.POINTER)]
            )]
        mock_c_writer = MockImplementationWriter(self.mockpaths, mockgen)

        # When
        mock_c_writer.write_implementation(self.cBuffer)

        # Then
        self.assertEqual(
            self.cBuffer.getvalue(),
            '#include "mock-mockable.h"\n'
            + boilerplate_includes
            + """pfstest_mock_define(mock_func1, "func1", 1);
void func1(char *__pfstest_arg_0)
{
    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func1,
                            NULL,
                            pfstest_the_pointer(__pfstest_arg_0));

    (void)__pfstest_return_value;
}

"""
        )

    def test_shouldWriteMultipleParameters(self):
        # Given
        mockgen = MagicMock()
        mockgen.mocks = [
            MockInfo(mockname = "mock_func1",
                     funcname = "func1",
                     prototype = "void func1(int __pfstest_arg_0, "
                     + "char *__pfstest_arg_1)",
                     return_text = "void",
                     return_hint = ReturnHint.VOID,
                     args_info = [ArgInfo('__pfstest_arg_0', ArgHint.BLOB),
                                  ArgInfo('__pfstest_arg_1',
                                          ArgHint.POINTER)]
            )]
        mock_c_writer = MockImplementationWriter(self.mockpaths, mockgen)

        # When
        mock_c_writer.write_implementation(self.cBuffer)

        # Then
        self.assertEqual(
            self.cBuffer.getvalue(),
            '#include "mock-mockable.h"\n'
            + boilerplate_includes
            + """pfstest_mock_define(mock_func1, "func1", 2);
void func1(int __pfstest_arg_0, char *__pfstest_arg_1)
{
    pfstest_value_t *__pfstest_return_value =
        pfstest_mock_invoke(mock_func1,
                            NULL,
                            pfstest_the_memory(&__pfstest_arg_0,
                                               sizeof(__pfstest_arg_0)),
                            pfstest_the_pointer(__pfstest_arg_1));

    (void)__pfstest_return_value;
}

"""
        )
