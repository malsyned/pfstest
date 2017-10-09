#include "pfstest.h"
#include "pfstest-cli.h"

static void do_nothing(void) {}


pfstest_plugin_define(do_nothing_plugin,
                      do_nothing, do_nothing, do_nothing);

int main(int argc, char *argv[])
{
    return pfstest_main(argc, argv);
}
