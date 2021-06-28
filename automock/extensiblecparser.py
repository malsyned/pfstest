# SPDX-License-Identifier: LGPL-2.1-or-later
# -*- coding: utf-8 -*-

# Includes code from pycparserext <http://github.com/inducer/pycparserext>
# pycparserext is licensed to you under the MIT/X Consortium license:
# 
# Copyright (c) 2009-18 Andreas Klöckner and Contributors.
# 
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

from pycparser.c_parser import CParser
from pycparser.c_lexer import CLexer

class ExtensibleCParser(CParser):
    def __init__(self,
                 storage_class_specifiers=[],
                 type_qualifiers=[],
                 type_specifiers=[],
                 function_specifiers=[],
                 lexer_base=CLexer,
                 **kwargs):

        self.lexer_class = type("ExtensibleCLexer", (CLexer,), dict())

        self.add_rule("storage_class_specifier", storage_class_specifiers,
                      self.p_storage_class_specifier)
        self.add_rule("type_qualifier", type_qualifiers,
                      self.p_type_qualifier)
        self.add_rule("function_specifier", function_specifiers,
                      self.p_function_specifier)
        self.add_rule("type_specifier_no_typeid", type_specifiers,
                      self.p_type_specifier_no_typeid)

        kwargs['lexer'] = self.lexer_class
        kwargs['lex_optimize'] = False
        kwargs['yacc_optimize'] = False
        #kwargs['yacc_debug']=True

        CParser.__init__(self, **kwargs)

    def add_rule(self, nonterminal, keywords, action):
        add_lexer_keywords(self.lexer_class, keywords)

        names = self.make_rule_names(nonterminal, len(keywords))
        for (name, keyword) in zip(names, keywords):
            def rule(self, p): action(p)
            rule.__doc__ = "%s : %s\n" % (nonterminal, keyword.upper())
            rule.__name__ = name
            setattr(self.__class__, rule.__name__, rule)

    def make_rule_names(self, production_type, n):
        return ["p_extensiblecparser_%s_%s" % (production_type, i)
                for i in range(n)]

    # GCC accepts empty structs and I find them very useful for making
    # bare bones headers for mocking yet-to-be-written modules.
    def p_struct_declaration_list_empty(self, p):
        """ struct_declaration_list : empty """
        p[0] = None

def add_lexer_keywords(cls, keywords):
    cls.keywords = cls.keywords + tuple(
            kw.upper() for kw in keywords)

    cls.keyword_map = cls.keyword_map.copy()
    cls.keyword_map.update(dict(
        (kw, kw.upper()) for kw in keywords))

    cls.tokens = cls.tokens + tuple(
            kw.upper() for kw in keywords)
