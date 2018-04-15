#!/usr/bin/env python3

import sys
from os import path
import re
import copy
from collections import namedtuple
sys.path.append(path.join(path.dirname(__file__), 'pycparser'))
from pycparser.c_ast import *

class MockPathHandler:
    def __init__(self, headerpath, outputpath):
        self.headerpath = headerpath
        self.mockheaderpath = outputpath + '.h'
        self.mockimplementationpath = outputpath + '.c'
        self.mockheaderrelpath = \
            path.relpath(self.mockheaderpath,
                         path.dirname(self.mockimplementationpath))
        self.headerrelpath = path.relpath(headerpath,
                                          path.dirname(self.mockheaderpath))
        self.guardmacro = self.make_guardname(self.mockheaderpath)

    def make_guardname(self, filename):
        return re.sub('[^A-Za-z_]', '_', path.basename(filename).upper())

class MockHeaderWriter:
    def __init__(self, mockpaths, mockgen):
        self.mockpaths = mockpaths
        self.mockgen = mockgen

    def write_header(self, ostream):
        ostream.write('#ifndef %s\n' % self.mockpaths.guardmacro)
        ostream.write('#define %s\n\n' % self.mockpaths.guardmacro)
        ostream.write('#include "%s"\n\n' % self.mockpaths.headerrelpath)
        ostream.write('#include "pfstest-mock.h"\n\n')

        for mock in self.mockgen.mocks:
            ostream.write('pfstest_mock_declare(%s);\n' % mock.mockname)
        ostream.write('\n')

        ostream.write('#endif /* !defined(%s) */\n'
                      % self.mockpaths.guardmacro)

class MockImplementationWriter:
    def __init__(self, mockpaths, mockgen):
        self.mockpaths = mockpaths
        self.mockgen = mockgen

    def write_implementation(self, ostream):
        self.write_mock_header_include(ostream)
        self.write_boilerplate_includes(ostream)
        for mock in self.mockgen.mocks:
            self.write_mock(ostream, mock)

    def write_mock_header_include(self, ostream):
        ostream.write('#include "%s"\n\n' % self.mockpaths.mockheaderrelpath)

    def write_boilerplate_includes(self, ostream):
        ostream.write('#include <stddef.h>\n')
        ostream.write('#include <string.h>\n\n')
        ostream.write('#include "pfstest-platform.h"\n')
        ostream.write('#include "pfstest-values.h"\n\n')

    def write_mock(self, ostream, mock):
        return_type_writer = self.return_type_writer_factory(
            mock.return_hint, mock.return_text)
        ostream.write('pfstest_mock_define(%s, "%s", %s);\n' %
                      (mock.mockname, mock.funcname,
                       len(mock.args_info)))
        ostream.write('%s\n' % mock.prototype)
        ostream.write('{\n')
        return_type_writer.declare_default_return(ostream)
        ostream.write('    pfstest_value_t *__pfstest_return_value =\n')
        ostream.write('        pfstest_mock_invoke(%s,\n' % mock.mockname)
        ostream.write('                            ');
        return_type_writer.create_default_return_argument(ostream)
        for arg_info in mock.args_info:
            ostream.write(',\n                            ')
            arg_type_writer = self.arg_type_writer_factory(arg_info.hint)
            arg_type_writer(ostream, arg_info.name)
        ostream.write(');\n\n')
        return_type_writer.return_result(ostream)
        ostream.write('}\n\n')

    def return_type_writer_factory(self, return_type_hint, return_type_text):
        if return_type_hint == ReturnHint.VOID:
            return self.ReturnTypeVoidWriter()
        elif return_type_hint == ReturnHint.POINTER:
            return self.ReturnTypePointerWriter(return_type_text)
        elif return_type_hint == ReturnHint.BLOB:
            return self.ReturnTypeBlobWriter(return_type_text)
        else:
            return self.ReturnTypePrimitiveWriter(return_type_text)

    class ReturnTypeVoidWriter():
        def declare_default_return(self, ostream):
            pass

        def create_default_return_argument(self, ostream):
            ostream.write('NULL')

        def return_result(self, ostream):
            ostream.write('    (void)__pfstest_return_value;\n')

    class ReturnTypePointerWriter():
        def __init__(self, return_type_text):
            self.return_type_text = return_type_text

        def declare_default_return(self, ostream):
            ostream.write('    %s __pfstest_default_return = NULL;\n\n'
                          % self.return_type_text)

        def create_default_return_argument(self, ostream):
            ostream.write('pfstest_the_pointer(__pfstest_default_return)')

        def return_result(self, ostream):
            ostream.write(('    return (%s)pfstest_value_data('
                           + '__pfstest_return_value);\n')
                          % self.return_type_text)

    class ReturnTypeBlobWriter():
        def __init__(self, return_type_text):
            self.return_type_text = return_type_text

        def declare_default_return(self, ostream):
            ostream.write('    %s __pfstest_default_return;\n'
                          % self.return_type_text)
            ostream.write('    memset(&__pfstest_default_return, 0, '
                          + 'sizeof(__pfstest_default_return));\n\n')

        def create_default_return_argument(self, ostream):
            ostream.write('pfstest_the_pointer(&__pfstest_default_return)')

        def return_result(self, ostream):
            ostream.write(('    return *(%s *)pfstest_value_data('
                           + '__pfstest_return_value);\n')
                          % self.return_type_text)

    class ReturnTypePrimitiveWriter():
        def __init__(self, return_type_text):
            self.return_type_text = return_type_text

        def declare_default_return(self, ostream):
            ostream.write('    %s __pfstest_default_return = 0;\n\n'
                          % self.return_type_text)

        def create_default_return_argument(self, ostream):
            ostream.write('pfstest_the_pointer(&__pfstest_default_return)')

        def return_result(self, ostream):
            ostream.write(('    return *(%s *)pfstest_value_data('
                           + '__pfstest_return_value);\n')
                          % self.return_type_text)

    def arg_type_writer_factory(self, arg_type_hint):
        if arg_type_hint == ArgHint.POINTER:
            return self.arg_type_writer_pointer
        else:
            return self.arg_type_writer_blob

    def arg_type_writer_pointer(self, ostream, arg_name):
        ostream.write('pfstest_the_pointer(%s)' % arg_name)

    def arg_type_writer_blob(self, ostream, arg_name):
        ostream.write('pfstest_the_pointer(&%s)' % arg_name)

class MockGenerator:
    def __init__(self, mpaths, cgen, ast):
        self.cgen = cgen
        self.mpaths = mpaths
        self.mocks = self.make_mocks(ast.ext)

    def make_mocks(self, ext):
        typedefs = {}
        seen_names = set()
        mocks = []

        for extdecl in ext:
            if self.ext_is_typedef(extdecl):
                self.update_typedefs(typedefs, extdecl)
            elif self.ext_declares_function(extdecl):
                funcdecl = extdecl.type
                if not self.function_in_file(funcdecl,
                                             self.mpaths.headerpath):
                    continue
                if self.function_is_variadic(funcdecl):
                    continue
                if self.extract_funcname(funcdecl) in seen_names:
                    continue

                mock = self.make_mock(typedefs, funcdecl)
                mocks.append(mock)
                seen_names.add(mock.funcname)
        return mocks

    def ext_is_typedef(self, extdecl):
        return isinstance(extdecl, Typedef)

    def update_typedefs(self, typedefs, extdecl):
        typedef_name = self.extract_typedecl(extdecl).declname
        typedefs[typedef_name] = extdecl.type

    def extract_typedecl(self, node):
        if isinstance(node, TypeDecl):
            return node
        else:
            return self.extract_typedecl(node.type)

    def ext_declares_function(self, extdecl):
        return (isinstance(extdecl, Decl)
                and isinstance(extdecl.type, FuncDecl))

    def function_in_file(self, funcdecl, path):
        return funcdecl.coord.file == path

    def function_is_variadic(self, funcdecl):
        return any(isinstance(param, EllipsisParam)
                   for param in funcdecl.args.params)

    def extract_funcname(self, funcdecl):
        return self.extract_typedecl(funcdecl).declname

    def make_mock(self, typedefs, funcdecl):
        # Make a copy so I can modify it with self.set_param_names()
        funcdecl = copy.deepcopy(funcdecl)
        funcname = self.extract_funcname(funcdecl)

        returntype = funcdecl.type
        typedef = self.find_typedef(typedefs, returntype)
        if typedef:
            return_hint = self.select_return_hint(typedef)
        else:
            return_hint = self.select_return_hint(returntype)
        returntextnode = self.make_return_text_node(returntype)

        params = funcdecl.args.params
        arg_names = self.arg_names(params)
        arg_hints = self.arg_hints(typedefs, params)
        self.set_param_names(params, arg_names, arg_hints)
        args_info = self.make_args_info(arg_names, arg_hints)

        return MockInfo(mockname = "mock_" + funcname,
                         funcname = funcname,
                         prototype = self.cgen.visit(funcdecl),
                         return_text = self.cgen.visit(returntextnode),
                         return_hint = return_hint,
                         args_info = args_info)

    def find_typedef(self, typedefs, type):
        if isinstance(type, TypeDecl):
            basetype = type.type
            if isinstance(basetype, IdentifierType):
                typename = ' '.join(basetype.names)
                if typename in typedefs:
                    return typedefs[typename]
        return None

    def select_return_hint(self, returntype):
        if isinstance(returntype, PtrDecl):
            return ReturnHint.POINTER
        elif isinstance(returntype, TypeDecl):
            basetype = returntype.type
            if isinstance(basetype, Struct):
                return ReturnHint.BLOB
            elif isinstance(basetype, Enum):
                return ReturnHint.PRIMITIVE
            elif isinstance(basetype, IdentifierType):
                if basetype.names == ['void']:
                    return ReturnHint.VOID
                else:
                    return ReturnHint.PRIMITIVE
            else:
                # FIXME: Thow a more specific exception
                raise Exception("Couldn't match return type to hint")

    # CGenerator won't correctly print a type-name if passed a
    # modifier like PtrDecl or ArrayDecl, unless it is wrapped in a
    # Typename or certain other kinds of nodes.
    def make_return_text_node(self, returntype):
        returntextnode = Typename(name=None,
                                  quals=[],
                                  type=copy.deepcopy(returntype))
        self.set_declname(returntextnode, None)
        return returntextnode

    def set_declname(self, node, name):
        typedecl = self.extract_typedecl(node)
        typedecl.declname = name

    def arg_names(self, params):
        return ['__pfstest_arg_%s' % i for i in range(len(params))]

    def arg_hints(self, typedefs, params):
        hints = []
        for param in params:
            paramtype = param.type
            typedef = self.find_typedef(typedefs, paramtype)
            if typedef:
                hints.append(self.select_arg_hint(typedef))
            else:
                hints.append(self.select_arg_hint(paramtype))
        return hints

    def select_arg_hint(self, paramtype):
        if isinstance(paramtype, TypeDecl):
            basetype = paramtype.type
            if isinstance(basetype, IdentifierType):
                if basetype.names == ['void']:
                    return None
                else:
                    return ArgHint.BLOB
            elif isinstance(basetype, Struct):
                return ArgHint.BLOB
            else:
                # FIXME: Thow a more specific exception
                raise Exception("Couldn't match param type to hint")
        elif isinstance(paramtype, PtrDecl):
            return ArgHint.POINTER

    def set_param_names(self, params, names, hints):
        for (param, name, hint) in zip(params, names, hints):
            if hint != None:    # Don't set a name for (void)
                self.set_declname(param, name)

    def make_args_info(self, names, hints):
        return [ArgInfo(name, hint)
                for (name, hint) in zip(names, hints)
                if hint != None] # Don't make ArgInfo for (void)

def enum(name, fields):
    class EnumValue:
        def __init__(self, clsname, name):
            self.repr = '%s.%s' % (clsname, name)

        def __repr__(self):
            return self.repr

    dict_ = dict((field, EnumValue(name, field))
                 for field in fields.split(' '))
    return type(name, (), dict_)

MockInfo = namedtuple('MockInfo',
                      'mockname funcname prototype return_text return_hint '
                      + 'args_info')
ReturnHint = enum('ReturnHint', 'VOID PRIMITIVE POINTER BLOB')

ArgInfo = namedtuple('ArgInfo', 'name hint')
ArgHint = enum('ArgHint', 'POINTER BLOB')

from argparse import ArgumentParser

argv_parser = ArgumentParser(
    description=
    'Generate a PFSTest mock from a preprocessed header file on stdin')
argv_parser.add_argument('headerpath', metavar='header-name', type=str,
                         help='The name of the original header file')
argv_parser.add_argument('mockroot', metavar='mock-template', type=str,
                         help='The initial part of the path to the '
                         + 'generated mock. automock.py will append '
                         + '.h and .c to this to generate the mock '
                         + 'source files')
argv_parser.add_argument('-s', '--storage-class-specifier',
                         metavar='specifier', type=str, action='append',
                         help='Additional storage class specifier to accept')
argv_parser.add_argument('-q', '--type-qualifier',
                         metavar='qualifier', type=str, action='append',
                         help='Additional type qualifier to accept')
argv_parser.add_argument('-t', '--type',
                         metavar='type', type=str, action='append',
                         help='Additional built-in type to accept')
argv_parser.add_argument('-f', '--function-specifier',
                         metavar='specifier', type=str, action='append',
                         help='Additional function specifier to accept')

if __name__ == "__main__":
    from sys import stdin
    from pycparser.c_generator import CGenerator
    from extensiblecparser import ExtensibleCParser

    args = argv_parser.parse_args()
    cparser = ExtensibleCParser(
        storage_class_specifiers=args.storage_class_specifier or [],
        type_qualifiers=args.type_qualifier or [],
        types=args.type or [],
        function_specifiers=args.function_specifier or [])
    ast = cparser.parse(stdin.read(), args.headerpath)
    mpaths = MockPathHandler(args.headerpath, args.mockroot)
    mg = MockGenerator(mpaths, CGenerator(), ast)
    hwriter = MockHeaderWriter(mpaths, mg)
    cwriter = MockImplementationWriter(mpaths, mg)

    with open(mpaths.mockheaderpath, 'w') as hstream:
        hwriter.write_header(hstream)
    with open(mpaths.mockimplementationpath, 'w') as cstream:
        cwriter.write_implementation(cstream)
