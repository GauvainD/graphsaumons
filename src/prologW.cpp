#include "/usr/share/gprolog/include/gprolog.h"
#include "graph.hpp"
#include "graph6.hpp"
#include <cstdio>

using namespace std;
using namespace phoeg;

Graph *gProlog;

int main(int argc, char *argv[])
{
    *gProlog = convertFromGraph6("D~{");
    Pl_Start_Prolog(argc, argv);
    int at = Pl_Find_Atom("thisisatest");
    printf("%d\n", at);
    //PlTerm args[] = {Pl_Mk_Integer(1), Pl_Mk_Integer(2)};
    PlTerm args[] = {Pl_Mk_Variable()};
    Pl_Query_Begin(PL_TRUE);
    int res = Pl_Query_Call(at, 1, args);
    printf("%d\n", res);
    //long i,j;
    //i = Rd_Integer(args[0]);
    //printf("maxhuv %d\n", i,j);
    Pl_Stop_Prolog();
    return 0;
}
