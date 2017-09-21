#include <stdio.h>

enum return_type {
    return_type_pointer,
    return_type_primitive,
    return_type_struct,
    return_type_void
};

enum arg_type {
    arg_type_pointer,
    arg_type_data
};

struct return_type_default_value_mapping
{
    enum return_type return_type;
    const char *default_value;
};

struct return_type_default_value_mapping return_type_default_values[] = {
    {return_type_pointer, "NULL"},
    {return_type_primitive, "0"},
    {return_type_struct, "{}"},
    {return_type_void, "#ERROR#"},
};

static const char *return_type_default_value(enum return_type return_type)
{
    int i;
    struct return_type_default_value_mapping *mapping;

    for (i = 0;
         i < (sizeof(return_type_default_values)
              / sizeof(return_type_default_values[0]));
         i++)
    {
        mapping = &return_type_default_values[i];
        if (mapping->return_type == return_type)
            return mapping->default_value;
    }

    return "#ERROR#";
}

static void write_mock(const char *function_name,
                       const char *prototype, const char *return_type_text,
                       enum return_type return_type,
                       enum arg_type *arg_types, const char **arg_names,
                       int nargs)
{
    int i;

    printf("pfstest_mock_define(mock_%s, \"%s\", %d);\n\n",
           function_name, function_name, nargs);

    printf("%s\n", prototype);
    printf("{\n");
    if (return_type != return_type_void)
    {
        printf("    %s __pfstest_default_return = %s;\n\n",
               return_type_text, return_type_default_value(return_type));
    }
    printf("    pfstest_value_t *__pfstest_return_value =\n");
    printf("        pfstest_mock_invoke(mock_%s,\n", function_name);
    printf("                            ");
    if (return_type == return_type_void) {
        printf("NULL");
    } else if (return_type == return_type_pointer) {
        printf("the_pointer(__pfstest_default_return)");
    } else {
        printf("the_pointer(&__pfstest_default_return)");
    }

    for (i = 0; i < nargs; i++) {
        enum arg_type arg_type = arg_types[i];
        const char *arg_name = arg_names[i];

        printf(",\n                            the_pointer(%s%s)",
               (arg_type == arg_type_data) ? "&" : "",
               arg_name);
    }
    printf(");\n\n");
                   
    if (return_type == return_type_void) {
        printf("    (void)__pfstest_return_value;\n");
    } else if (return_type == return_type_pointer) {
        printf("    return (%s)pfstest_value_data(__pfstest_return_value);\n",
               return_type_text);
    } else {
        printf("    return *(%s *)pfstest_value_data(__pfstest_return_value);\n",
               return_type_text);
    }

    printf("}\n\n");
}

int main(void)
{
    enum arg_type dep_func1_arg_types[] = {arg_type_data};
    const char *dep_func1_arg_names[] = {"i"};
        
    enum arg_type dep_func2_arg_types[] = {arg_type_data, arg_type_pointer};
    const char *dep_func2_arg_names[] = {"i", "s"};

    write_mock("dep_func1",
               "int dep_func1(int i)", "int",
               return_type_primitive,
               dep_func1_arg_types, dep_func1_arg_names, 1);
    
    write_mock("dep_func2",
               "void dep_func2(int i, char *s)", "void",
               return_type_void,
               dep_func2_arg_types, dep_func2_arg_names, 2);

    write_mock("dep_func3",
               "char *dep_func3(void)", "char *",
               return_type_pointer,
               NULL, NULL, 0);

    return 0;
}
