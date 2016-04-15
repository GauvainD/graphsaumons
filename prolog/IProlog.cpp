#include "/usr/share/gprolog/include/gprolog.h"

using namespace std;

extern "C"
{
    PlBool edge(PlLong i, PlLong j)
    {
        return PL_FALSE;
    }
}
