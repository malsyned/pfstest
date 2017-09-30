# Borrows code from pycparserext <http://github.com/inducer/pycparserext>

from pycparser.c_parser import CParser
from pycparser.c_lexer import CLexer

class ExtensibleCParser(CParser):
    def __init__(self,
                 storage_class_specifiers=[],
                 type_qualifiers=[],
                 types=[],
                 function_specifiers=[],
                 lexer_base=CLexer,
                 **kwargs):
        self.initial_type_symbols = types

        self.lexer_class = type("ExtensibleCLexer", (CLexer,), dict())

        self.add_rule("storage_class_specifier", storage_class_specifiers)
        self.add_rule("type_qualifier", type_qualifiers)
        self.add_rule("function_specifier", function_specifiers)

        kwargs['lexer'] = self.lexer_class
        kwargs['lex_optimize'] = False
        kwargs['yacc_optimize'] = False
        #kwargs['yacc_debug']=True

        CParser.__init__(self, **kwargs)

    def add_rule(self, nonterminal, keywords):
        add_lexer_keywords(self.lexer_class, keywords)

        names = self.make_rule_names(nonterminal, len(keywords))
        for (name, keyword) in zip(names, keywords):
            def rule(self, p): p[0] = p[1]
            rule.__doc__ = "%s : %s\n" % (nonterminal, keyword.upper())
            rule.__name__ = name
            setattr(self.__class__, rule.__name__, rule)

    def make_rule_names(self, production_type, n):
        return ["p_extensiblecparser_%s_%s" % (production_type, i)
                for i in range(n)]

    # Override the parse function to initialize _scope_stack with the
    # listed types
    def parse(self, text, filename='', debuglevel=0):
        self.clex.filename = filename
        self.clex.reset_lineno()
        # Apart from this line, this method should be a copy of
        # CParser.parse
        self._scope_stack = [
            dict((tpsym, 1) for tpsym in self.initial_type_symbols)]
        self._last_yielded_token = None
        return self.cparser.parse(
                input=text,
                lexer=self.clex,
                debug=debuglevel)

def add_lexer_keywords(cls, keywords):
    cls.keywords = cls.keywords + tuple(
            kw.upper() for kw in keywords)

    cls.keyword_map = cls.keyword_map.copy()
    cls.keyword_map.update(dict(
        (kw, kw.upper()) for kw in keywords))

    cls.tokens = cls.tokens + tuple(
            kw.upper() for kw in keywords)
