# SPDX-License-Identifier: LGPL-2.1-or-later

from unittest import TestCase

from compat import StringIO

from extensiblecparser import ExtensibleCParser

# pycparser takes a long time to initialize, so create a singleton
# once and run different tests on it rather than creating a more
# limited one in each test case
cparser = ExtensibleCParser(storage_class_specifiers=['overlay'],
                            type_qualifiers=['near','far'],
                            type_specifiers=['_Float32'],
                            function_specifiers=['__inline', '_Noreturn'],

                            lex_optimize = False,
                            yacc_optimize = False)

class ExtensibleCParserTests(TestCase):
    def assertASTShows(self, ast, string):
        showbuf = StringIO()
        ast.show(buf=showbuf, nodenames=True, attrnames=True)
        self.assertEqual(showbuf.getvalue(), string)

    def test_shouldAcceptNewStorageClassSpecifiers(self):
        # such as: auto, register, static, extern
        ast = cparser.parse('void foo(overlay char a);')

        self.assertASTShows(ast, """FileAST: 
  Decl <ext[0]>: name=foo, quals=[], storage=[], funcspec=[]
    FuncDecl <type>: 
      ParamList <args>: 
        Decl <params[0]>: name=a, quals=[], storage=['overlay'], funcspec=[]
          TypeDecl <type>: declname=a, quals=[]
            IdentifierType <type>: names=['char']
      TypeDecl <type>: declname=foo, quals=[]
        IdentifierType <type>: names=['void']
""")

    def test_shouldAcceptNewTypeQualifiers(self):
        # such as: const, volatile, restrict
        ast = cparser.parse('void foo(near char *a, far int *b);')

        self.assertASTShows(ast, """FileAST: 
  Decl <ext[0]>: name=foo, quals=[], storage=[], funcspec=[]
    FuncDecl <type>: 
      ParamList <args>: 
        Decl <params[0]>: name=a, quals=['near'], storage=[], funcspec=[]
          PtrDecl <type>: quals=[]
            TypeDecl <type>: declname=a, quals=['near']
              IdentifierType <type>: names=['char']
        Decl <params[1]>: name=b, quals=['far'], storage=[], funcspec=[]
          PtrDecl <type>: quals=[]
            TypeDecl <type>: declname=b, quals=['far']
              IdentifierType <type>: names=['int']
      TypeDecl <type>: declname=foo, quals=[]
        IdentifierType <type>: names=['void']
""")

    def test_shouldAcceptNewTypeSpecifiers(self):
        ast = cparser.parse(
            '_Complex _Float32 cacosf32;')

        self.assertASTShows(ast, """FileAST: 
  Decl <ext[0]>: name=cacosf32, quals=[], storage=[], funcspec=[]
    TypeDecl <type>: declname=cacosf32, quals=[]
      IdentifierType <type>: names=['_Complex', '_Float32']
""")

    def test_shouldAcceptNewFunctionSpecifiers(self):
        # such as: inline
        ast = cparser.parse('_Noreturn void foo(void);')

        self.assertASTShows(ast, """FileAST: 
  Decl <ext[0]>: name=foo, quals=[], storage=[], funcspec=['_Noreturn']
    FuncDecl <type>: 
      ParamList <args>: 
        Typename <params[0]>: name=None, quals=[]
          TypeDecl <type>: declname=None, quals=[]
            IdentifierType <type>: names=['void']
      TypeDecl <type>: declname=foo, quals=[]
        IdentifierType <type>: names=['void']
""")

    def test_shouldAcceptEmptyStructs(self):
        # As long as this doesn't throw an exception, I'm satisfied
        ast = cparser.parse('struct foo {};')
