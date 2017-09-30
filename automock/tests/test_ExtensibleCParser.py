from unittest import TestCase

# pycparser.c_ast.FileAST doesn't have a useful __eq__ method for use
# with assertEqual. However, the pycparser test suite contains utility
# functions for converting ast subtrees to nested lists for comparison
import sys, os.path
sys.path.append(os.path.join(os.path.dirname(__file__),
                             '..','pycparser','tests'))
from test_c_parser import expand_decl

from extensiblecparser import ExtensibleCParser

# pycparser takes a long time to initialize, so create a singleton
# once and run different tests on it rather than creating a more
# limited one in each test case
cparser = ExtensibleCParser(storage_class_specifiers=['overlay'],
                            type_qualifiers=['near','far'],
                            types=['__builtin_va_list', 'a_builtin_type'],
                            function_specifiers=['__inline', '_Noreturn'],

                            lex_optimize = False,
                            yacc_optimize = False)

class ExtensibleCParserTests(TestCase):
    def test_shouldAcceptNewStorageClassSpecifiers(self):
        pass

    def test_shouldAcceptNewTypeQualifiers(self):
        pass

    def test_shouldAcceptNewTypes(self):
        ast = cparser.parse('__builtin_va_list va; a_builtin_type abt;')

        self.assertEqual(expand_decl(ast.ext[0]),
                         ['Decl', 'va',
                          ['TypeDecl',
                           ['IdentifierType', ['__builtin_va_list']]]])
        self.assertEqual(expand_decl(ast.ext[1]),
                         ['Decl', 'abt',
                          ['TypeDecl',
                           ['IdentifierType', ['a_builtin_type']]]])

    def test_shouldAcceptNewFunctionSpecifiers(self):
        pass

    # CHECKME: Do I want to support new type specifiers, or just new
    # types?
