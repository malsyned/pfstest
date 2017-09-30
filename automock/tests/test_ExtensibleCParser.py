from unittest import TestCase

from compat import StringIO

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
    def setUp(self):
        self.showbuf = StringIO()

    def test_shouldAcceptNewStorageClassSpecifiers(self):
        pass

    def test_shouldAcceptNewTypeQualifiers(self):
        pass

    def test_shouldAcceptNewTypes(self):
        ast = cparser.parse('__builtin_va_list va; a_builtin_type abt;')
        ast.show(buf=self.showbuf, nodenames=True, attrnames=True)

        self.assertEqual(self.showbuf.getvalue(),
                         """FileAST: 
  Decl <ext[0]>: name=va, quals=[], storage=[], funcspec=[]
    TypeDecl <type>: declname=va, quals=[]
      IdentifierType <type>: names=['__builtin_va_list']
  Decl <ext[1]>: name=abt, quals=[], storage=[], funcspec=[]
    TypeDecl <type>: declname=abt, quals=[]
      IdentifierType <type>: names=['a_builtin_type']
""")

    def test_shouldAcceptNewFunctionSpecifiers(self):
        pass

    # CHECKME: Do I want to support new type specifiers, or just new
    # types?
