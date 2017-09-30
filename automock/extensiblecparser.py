# Borrows code from pycparserext <http://github.com/inducer/pycparserext>

from pycparser.c_parser import CParser

class ExtensibleCParser(CParser):
    def __init__(self,
                 storage_class_specifiers=[],
                 type_qualifiers=[],
                 types=[],
                 function_specifiers=[],
                 **kwargs):
        self.initial_type_symbols = types
        CParser.__init__(self, **kwargs)

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
