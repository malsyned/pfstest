from unittest import TestCase

from pycparser.c_parser import CParser
from pycparser.c_generator import CGenerator

from automock import MockGenerator
from automock import MockInfo, ReturnHint
from automock import ArgInfo, ArgHint

from os import path

# CParser() takes about a second to run on my machine, so create it
# only once instead of in setUp() for every test
cparser = CParser()
cgen = CGenerator()
emptyast = cparser.parse('')

class MockGeneratorTests(TestCase):
    def test_souldStoreHeaderName(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h", emptyast)
        # When
        hname = mgen.headername
        # Then
        self.assertEqual(hname, "mockable.h")

    def test_shouldRemoveDirectoryFromHeaderName(self):
        # Given
        headername = path.join("..", "foo", "theheader.h")
        mgen = MockGenerator(cgen, headername, emptyast)
        # When
        hname = mgen.headername
        # Then
        self.assertEqual(hname, "theheader.h")

    def test_shouldCreateGuardMacro(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h", emptyast)
        # When
        gm = mgen.guardmacro
        # Then
        self.assertEqual(gm, "_PFSTEST_MOCK_MOCKABLE_H")

    def test_shouldStripDirNameFromGuardMacro(self):
        # Given
        headername = path.join("..", "foo", "theheader.h")
        mgen = MockGenerator(cgen, headername, emptyast)
        # When
        gm = mgen.guardmacro
        # Then
        self.assertEqual(gm, "_PFSTEST_MOCK_THEHEADER_H")

    def test_shouldCleanNonIdentifierCharactersFromGuardMacro(self):
        # Given
        headername = "some header-file?with&bad\"chars'.h"
        mgen = MockGenerator(cgen, headername, emptyast)
        # When
        gm = mgen.guardmacro
        # Then
        self.assertEqual(gm,
                         "_PFSTEST_MOCK_SOME_HEADER_FILE_WITH_BAD_CHARS__H")

    def test_shouldCreateMockHeaderName(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h", emptyast)
        # When
        mockh = mgen.mockheadername
        # Then
        self.assertEqual(mockh, "mock-mockable.h")

    def test_shouldRemoveDirectoryFromMockHeaderName(self):
        # Given
        headername = path.join("..", "foo", "theheader.h")
        mgen = MockGenerator(cgen, headername, emptyast)
        # When
        mockh = mgen.mockheadername
        # Then
        self.assertEqual(mockh, "mock-theheader.h")

    def test_shouldGenerateMockFromOtherwiseEmptyHeader(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse("void func1(void);"))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = "void func1(void)",
                                   return_text = "void",
                                   return_hint = ReturnHint.VOID,
                                   args_info = [])
                         ])

    def test_shouldGenerateMultipleMocks(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse("void func1(void);" +
                                           "void func2(void);"))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
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
                                   args_info = [])
                         ])


    def test_shouldHandleSimplePrimitiveReturnType(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse("int func1(void);"))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = "int func1(void)",
                                   return_text = "int",
                                   return_hint = ReturnHint.PRIMITIVE,
                                   args_info = [])
                         ])

    def test_shouldHandleCompoundTypeSpecifierInReturnType(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse("unsigned long int func1(void);"))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = \
                                   "unsigned long int func1(void)",
                                   return_text = "unsigned long int",
                                   return_hint = ReturnHint.PRIMITIVE,
                                   args_info = [])
                         ])

    def test_shouldHandlePointerReturnType(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse("char *func1(void);"))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = "char *func1(void)",
                                   return_text = "char *",
                                   return_hint = ReturnHint.POINTER,
                                   args_info = [])
                         ])

    def test_shouldHandleStructReturnType(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse("struct foo func1(void);"))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = "struct foo func1(void)",
                                   return_text = "struct foo",
                                   return_hint = ReturnHint.BLOB,
                                   args_info = [])
                         ])

    def test_shouldHandlePrimitiveParam(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse("void func1(int);"))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = \
                                   "void func1(int __pfstest_arg_0)",
                                   return_text = "void",
                                   return_hint = ReturnHint.VOID,
                                   args_info = [ArgInfo("__pfstest_arg_0",
                                                        ArgHint.BLOB)])
                         ])

    def test_shouldHandlePointerParam(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse("void func1(char *);"))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = \
                                   "void func1(char *__pfstest_arg_0)",
                                   return_text = "void",
                                   return_hint = ReturnHint.VOID,
                                   args_info = [ArgInfo("__pfstest_arg_0",
                                                        ArgHint.POINTER)])
                         ])

    def test_shouldHandleStructParam(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse("void func1(struct foo bar);"))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = \
                                   "void func1(struct foo __pfstest_arg_0)",
                                   return_text = "void",
                                   return_hint = ReturnHint.VOID,
                                   args_info = [ArgInfo("__pfstest_arg_0",
                                                        ArgHint.BLOB)])
                         ])

    def test_shouldHandleMultipleParams(self):
        # Given
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse("void func1(int, char);"))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = \
                                   "void func1(int __pfstest_arg_0, " \
                                   + "char __pfstest_arg_1)",
                                   return_text = "void",
                                   return_hint = ReturnHint.VOID,
                                   args_info = [ArgInfo("__pfstest_arg_0",
                                                        ArgHint.BLOB),
                                                ArgInfo("__pfstest_arg_1",
                                                        ArgHint.BLOB)])
                         ])

    def test_shouldIgnoreNonFunctionDeclarations(self):
        # Given
        source = """
            extern int i;
            struct foo {int a; int b;};
            enum bar {A, B};
            union baz {int A; char B;};
            inline int ifunc(void) { return 0; }
            void func1(void);
        """
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse(source))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = "void func1(void)",
                                   return_text = "void",
                                   return_hint = ReturnHint.VOID,
                                   args_info = [])
                         ])

    def test_shouldUseTypedefsForReturnHints(self):
        # Given
        source = """
            typedef char *charp;
            charp func1(void);
        """
        mgen = MockGenerator(cgen, "mockable.h",
                             cparser.parse(source))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = "charp func1(void)",
                                   return_text = "charp",
                                   return_hint = ReturnHint.POINTER,
                                   args_info = [])
                         ])
