from unittest import TestCase

from compat import MagicMock

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
defaulthname = "../mockable.h"

class MockGeneratorTests(TestCase):
    def setUp(self):
        self.maxDiff = None
        self.mpaths = MagicMock()
        self.mpaths.headerpath = defaulthname

    def test_shouldGenerateMockFromOtherwiseEmptyHeader(self):
        # Given
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse("void func1(void);",
                                           defaulthname))
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
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse("void func1(void);" +
                                           "void func2(void);",
                                           defaulthname))
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
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse("int func1(void);",
                                           defaulthname))
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
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse("unsigned long int func1(void);",
                                           defaulthname))
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
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse("char *func1(void);",
                                           defaulthname))
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
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse("struct foo func1(void);",
                                           defaulthname))
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
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse("void func1(int);",
                                           defaulthname))
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
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse("void func1(char *);",
                                           defaulthname))
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
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse("void func1(struct foo bar);",
                                           defaulthname))
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
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse("void func1(int, char);",
                                           defaulthname))
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
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse(source, defaulthname))
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
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse(source, defaulthname))
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

    def test_shouldUseTypedefsForArgHints(self):
        # Given
        source = """
            typedef char *charp;
            void func1(charp cp);
        """
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse(source, defaulthname))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks,
                         [MockInfo(mockname = "mock_func1",
                                   funcname = "func1",
                                   prototype = \
                                   "void func1(charp __pfstest_arg_0)",
                                   return_text = "void",
                                   return_hint = ReturnHint.VOID,
                                   args_info = [ArgInfo("__pfstest_arg_0",
                                                        ArgHint.POINTER)])
                         ])

    def test_shouldIgnoreFuncDeclsFromIncludedFiles(self):
        # Given
        source = """
            # 1 "another-header.h"
            int func2(int);
            # 1 "../mockable.h"
            void func1(void);
        """
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse(source, defaulthname))
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

    def test_shouldSkipVariadicFunctions(self):
        # Given
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse('void foo(int, ...);',
                                           defaulthname))
        # When
        mocks = mgen.mocks
        # Then
        self.assertEqual(mocks, [])

    def test_shouldSkipDuplicateFunctionDeclarations(self):
        source = """
            void func1(void);
            void func1(void);
        """
        mgen = MockGenerator(self.mpaths, cgen,
                             cparser.parse(source, defaulthname))
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
