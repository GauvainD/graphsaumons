#include "graph.hpp"
#include "invariants.hpp"
#include "/usr/share/gprolog/include/gprolog.h"
#include <cstdio>

using namespace std;
using namespace phoeg;

extern Graph *gProlog;

extern "C"
{
    PlBool node(PlFIOArg *i)
    {
        int n = order(*gProlog);
        if (!i->is_var)
        {
            Pl_No_More_Choice();
            if (i->value.l >= 0 && i->value.l < n)
            {
                return PL_TRUE;
            }
            return PL_FALSE;
        }
        else
        {
            long *p = Pl_Get_Choice_Buffer(long*);
            if (!Pl_Get_Choice_Counter())
            {
                *p = 0;
            }
            if (*p < n)
            {
                i->value.l = *p;
                i->unify = PL_TRUE;
                *p++;
                return PL_TRUE;
            }
            else
            {
                Pl_No_More_Choice();
                return PL_FALSE;
            }
        }
    }

    PlBool edge(PlFIOArg *i, PlFIOArg *j)
    {
        int n = order(*gProlog);
        if (!i->is_var && !j->is_var)
        {
            Pl_No_More_Choice();
            if (edge(i->value.l, j->value.l, *gProlog).second)
            {
                return PL_TRUE;
            }
            return PL_FALSE;
        }
        else if (!j->is_var)
        {
            long *buf = Pl_Get_Choice_Buffer(long *);
            if (!Pl_Get_Choice_Counter())
            {
                buf[0] = 0;
            }
            while (buf[0] < n && edge(j->value.l,buf[0],*gProlog).second)
            {
                buf[0]++;
            }
            if (buf[0] >= n)
            {
                Pl_No_More_Choice();
                return PL_FALSE;
            }
            i->value.l = buf[0];
            buf[0]++;
            i->unify=PL_TRUE;
            return PL_TRUE;
        }
        else if (!i->is_var)
        {
            long *buf = Pl_Get_Choice_Buffer(long *);
            if (!Pl_Get_Choice_Counter())
            {
                buf[0] = 0;
            }
            while (buf[0] < n && edge(i->value.l,buf[0],*gProlog).second)
            {
                buf[0]++;
            }
            if (buf[0] >= n)
            {
                Pl_No_More_Choice();
                return PL_FALSE;
            }
            j->value.l = buf[0];
            buf[0]++;
            j->unify=PL_TRUE;
            return PL_TRUE;
        }
        else
        {
            long *buf = Pl_Get_Choice_Buffer(long *);
            if (!Pl_Get_Choice_Counter())
            {
                buf[0] = 0;
                buf[1] = 0;
            }
            while (buf[1] < n && !edge(buf[0],buf[1],*gProlog).second)
            {
                 buf[1]++;
            }
            while (buf[0] < n && !edge(buf[0],buf[1],*gProlog).second)
            {
                buf[0]++;
                buf[1] = 0;
                while (buf[1] < n && !edge(buf[0],buf[1],*gProlog).second)
                {
                    buf[1]++;
                }
            }
            if (buf[0] >= n)
            {
                Pl_No_More_Choice();
                return PL_FALSE;
            }
            i->value.l = buf[0];
            i->unify = PL_TRUE;
            j->value.l = buf[1];
            j->unify = PL_TRUE;
            buf[1]++;
            if (buf[1] >= n)
            {
                buf[1] = 0;
                buf[0]++;
            }
            return PL_TRUE;
        }
    }

    PlBool commonNeighbor(PlLong u, PlLong v, PlFIOArg *h)
    {
        if (!h->is_var)
        {
            Pl_No_More_Choice();
            if (edge(u,h->value.l,*gProlog).second && edge(v,h->value.l,*gProlog).second)
            {
                return PL_TRUE;
            }
            return PL_FALSE;
        }
        else
        {
            long *w = Pl_Get_Choice_Buffer(long *);
            if (!Pl_Get_Choice_Counter())
            {
                *w = 0;
            }
            long n = order(*gProlog);
            while (*w < n && (!edge(u,*w,*gProlog).second || !edge(v,*w,*gProlog).second))
            {
                *w++;
            }
            if (*w >= n)
            {
                Pl_No_More_Choice();
                return PL_FALSE;
            }
            h->value.l = *w;
            h->unify = PL_TRUE;
            *w++;
            return PL_TRUE;
        }
    }

    PlBool properNeighbor(PlLong u, PlLong v, PlFIOArg *h)
    {
        if (!h->is_var)
        {
            Pl_No_More_Choice();
            if (edge(u,h->value.l,*gProlog).second && !edge(v,h->value.l,*gProlog).second)
            {
                return PL_TRUE;
            }
            return PL_FALSE;
        }
        else
        {
            long *w = Pl_Get_Choice_Buffer(long *);
            if (!Pl_Get_Choice_Counter())
            {
                *w = 0;
            }
            long n = order(*gProlog);
            while (*w < n && (!edge(u,*w,*gProlog).second || edge(v,*w,*gProlog).second))
            {
                *w++;
            }
            if (*w >= n)
            {
                Pl_No_More_Choice();
                return PL_FALSE;
            }
            h->value.l = *w;
            h->unify = PL_TRUE;
            *w++;
            return PL_TRUE;
        }
    }
}
