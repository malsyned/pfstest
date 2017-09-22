import re

class MockHeaderWriter:
    def __init__(self, mockgen):
        self.mockgen = mockgen

    def write_header(self, ostream):
        ostream.write('#ifndef %s\n' % self.mockgen.guardmacro)
        ostream.write('#define %s\n\n' % self.mockgen.guardmacro)
        ostream.write('#include "%s"\n\n' % self.mockgen.headername)
        ostream.write('#include "pfstest-mock.h"\n\n')

        for mock in self.mockgen.mocks:
            ostream.write('pfstest_mock_declare(%s);\n' % mock.mockname)
        ostream.write('\n')

        ostream.write('#endif /* !defined(%s) */\n' % self.mockgen.guardmacro)

class MockImplementationWriter:
    def __init__(self, mockgen):
        self.mockgen = mockgen

    def write_implementation(self, ostream):
        self.write_header_include(ostream)
        self.write_boilerplate_includes(ostream)
        for mock in self.mockgen.mocks:
            self.write_mock(ostream, mock)

    def write_header_include(self, ostream):
        ostream.write('#include "%s"\n\n' % self.mockgen.mockheadername)

    def write_boilerplate_includes(self, ostream):
        ostream.write('#include <stddef.h>\n\n')
        ostream.write('#include "pfstest-platform.h"\n')
        ostream.write('#include "pfstest-values.h"\n\n')

    def write_mock(self, ostream, mock):
        return_type_writer = return_type_writer_factory(
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
        ostream.write(');\n\n')
        return_type_writer.return_result(ostream)
        ostream.write('}\n\n')

def return_type_writer_factory(return_type_hint, return_type_text):
    if (return_type_hint == ReturnHint.VOID):
        return ReturnTypeVoidWriter()
    else:
        return ReturnTypePrimitiveWriter(return_type_text)

class ReturnTypeVoidWriter():
    def declare_default_return(self, ostream):
        pass

    def create_default_return_argument(self, ostream):
        ostream.write('NULL')

    def return_result(self, ostream):
        ostream.write('    (void)__pfstest_return_value;\n')

class ReturnTypePrimitiveWriter():
    def __init__(self, return_type_text):
        self.return_type_text = return_type_text

    def declare_default_return(self, ostream):
        ostream.write('    %s __pfstest_default_return = 0;\n\n'
                      % self.return_type_text)

    def create_default_return_argument(self, ostream):
        ostream.write('the_pointer(&__pfstest_default_return)')

    def return_result(self, ostream):
        ostream.write('    return *(%s *)__pfstest_return_value;\n'
                      % self.return_type_text)

class MockGenerator:
    def __init__(self, cgen, headername, ast):
        self.cgen = cgen
        self.headername = headername
        self.mockheadername = "mock-" + headername
        self.guardmacro = self.make_guardname(headername)
        self.mocks = [self.make_mock(decl) for decl in ast.ext]

    def make_guardname(self, filename):
        return "_PFSTEST_MOCK_" + re.sub('\.', '_', filename.upper())

    def make_mock(self, decl):
        funcdecl = decl.type
        returntype = funcdecl.type
        idtype = returntype.type
        isvoid = idtype.names == ['void']
        if (isvoid):
            return_hint = ReturnHint.VOID
        else:
            return_hint = ReturnHint.PRIMITIVE
        return MockInfo(mockname = "mock_" + decl.name,
                         funcname = decl.name,
                         prototype = self.cgen.visit(decl),
                         return_text = decl.type.type.type.names[0],
                         return_hint = return_hint,
                         args_info = [])

class ReturnHint:
    VOID = 1
    PRIMITIVE = 2
    POINTER = 3
    BLOB = 4

class ArgHint:
    POINTER = 1
    BLOB = 2

from collections import namedtuple

MockInfo = namedtuple('MockInfo',
                      'mockname funcname prototype return_text return_hint '
                      + 'args_info')

ArgInfo = namedtuple('ArgInfo', 'name hint')

if __name__ == "__main__":
    import sys
    from sys import stdout
    sys.path.append('pycparser')
    import pycparser
    from pycparser.c_generator import CGenerator

    header = sys.argv[1]
    ast = pycparser.parse_file(header, use_cpp=True)
    mg = MockGenerator(CGenerator(), header, ast)
    hwriter = MockHeaderWriter(mg)
    cwriter = MockImplementationWriter(mg)

    stdout.write('H File:\n======\n\n')
    hwriter.write_header(stdout)
    stdout.write('\n\nC File:\n======\n\n')
    cwriter.write_implementation(stdout)
