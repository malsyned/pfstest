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

    def test_shouldAcceptNewTypes(self):
        # CHECKME: Do I want to support new type specifiers (such as:
        # int, char, short, unsigned), or just new types?
        ast = cparser.parse('__builtin_va_list va; a_builtin_type abt;')

        self.assertASTShows(ast, """FileAST: 
  Decl <ext[0]>: name=va, quals=[], storage=[], funcspec=[]
    TypeDecl <type>: declname=va, quals=[]
      IdentifierType <type>: names=['__builtin_va_list']
  Decl <ext[1]>: name=abt, quals=[], storage=[], funcspec=[]
    TypeDecl <type>: declname=abt, quals=[]
      IdentifierType <type>: names=['a_builtin_type']
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
