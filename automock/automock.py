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
        ostream.write('#include "%s"\n\n' % self.mockgen.mockheadername)
        ostream.write('#include <stddef.h>\n\n')
        ostream.write('#include "pfstest-platform.h"\n')
        ostream.write('#include "pfstest-values.h"\n\n')
        for mock in self.mockgen.mocks:
            ostream.write('pfstest_mock_define(%s, "%s", %s);\n' %
                           (mock.mockname, mock.funcname,
                            len(mock.args_info)))
            ostream.write('%s\n' % mock.prototype)
            ostream.write('{\n')
            ostream.write('    pfstest_value_t *__pfstest_return_value =\n')
            ostream.write('        pfstest_mock_invoke(%s,\n' % mock.mockname)
            ostream.write('                            ');
            ostream.write('NULL')
            ostream.write(');\n\n')
            ostream.write('    (void)__pfstest_return_value;\n')
            ostream.write('}\n\n')

class MockGenerator:
    def __init__(self, cgen, headername, ast):
        self.cgen = cgen
        self.headername = headername
        self.mockheadername = "mock-" + headername
        self.guardmacro = self.make_guardname(headername)
        self.mocks = []
        if (ast.ext):
            decl = ast.ext[0]
            self.mocks.append(self.make_mock(decl))

    def make_guardname(self, filename):
        return "_PFSTEST_MOCK_" + re.sub('\.', '_', filename.upper())

    def make_mock(self, decl):
        return MockInfo(mockname = "mock_" + decl.name,
                         funcname = decl.name,
                         prototype = self.cgen.visit(decl),
                         return_text = decl.type.type.type.names[0],
                         return_hint = ReturnHint.VOID,
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
