import sys

sys.path.extend(['/home/malsyned/packages/pycparser'])

from sys import stdout
from os import path
from pycparser import c_parser, c_ast, c_generator, parse_file
import re

def declare_mocks(ast):
    for node in ast.ext:
        nodetype = node.type
        if not isinstance(nodetype, c_ast.FuncDecl):
            return
        stdout.write('pfstest_mock_declare(mock_%s);\n' % node.name)
        

def guard_macro_name(filename):
    return ("MOCK_%s"
            % re.sub('[^A-Za-z_]', '_', path.basename(filename)).upper())

def start_guards(filename):
    macro = guard_macro_name(filename)
    stdout.write('#ifndef %s\n' % macro)
    stdout.write('#define %s\n\n' % macro)

def do_h_includes(filename):
    stdout.write('#include "%s"\n\n' % filename)
    stdout.write('#include "pfstest-mock.h"\n\n')

def end_guards(filename):
    stdout.write('\n#endif /* !defined(%s) */\n' % guard_macro_name(filename))

def do_c_includes():
    stdout.write('#include %s\n\n#include %s\n\n#include %s\n#include %s\n\n'
                 % ('"mock-dep.h"',
                    '<stddef.h>',
                    '"pfstest-platform.h"', '"pfstest-values.h"'))

def define_mocks(ast):
    for node in ast.ext:
        cgen = c_generator.CGenerator()
        nodetype = node.type
        if not isinstance(nodetype, c_ast.FuncDecl):
            return
        param_list = nodetype.args.params
        return_type = nodetype.type
        arg_count = len(param_list)
        stdout.write('pfstest_mock_define(mock_%s, "%s", %s);\n\n'
                     % (node.name, node.name, arg_count))
        stdout.write('%s\n{\n' % cgen.visit(node))
        stdout.write('}\n\n')

if __name__ == "__main__":
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    else:
        filename = 'dep.h'

    ast = parse_file(filename, use_cpp=True)

    stdout.write('%s:\n\n' % filename)
    start_guards(filename)
    do_h_includes(filename)
    declare_mocks(ast)
    end_guards(filename)

    stdout.write('%s\n%s.c:\n\n' % ('=' * 40, path.splitext(filename)[0]))

    do_c_includes()
    define_mocks(ast)
