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
    # Known fields based on unit tests so far:
    # guardmacro
    # headername: The original header file the mock was generated from
    # mockheadername: The name of the generated header for the mock
    # mocks: Metadata about the functions to be mocked in MockInfo objects
    pass

class ReturnHint:
    VOID = 1
    PRIMITIVE = 2
    POINTER = 3
    BLOB = 4

class ArgHint:
    POINTER = 1
    BLOB = 2

class MockInfo:
    def __init__(self, mockname, funcname, prototype, return_text,
                 return_hint, args_info):
        self.mockname = mockname
        self.funcname = funcname
        self.prototype = prototype
        self.return_text = return_text
        self.return_hint = return_hint
        self.args_info = args_info

class ArgInfo:
    def __init__(self, name, hint):
        self.name = name
        self.hint = hint
