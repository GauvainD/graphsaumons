#ifndef NAUTY_INTERFACE_HPP
#define NAUTY_INTERFACE_HPP

#include <cstdio>
#include <string>

#include "graph.hpp"
#include "nauty.h"

namespace phoeg
{
namespace detail
{
/**
 * Fill the array by adding the edges of the graph g
 * @param g the graph to put in array
 * @param n the order of g
 * @param m the size of g
 * @param array the array to fill
 */
void fillArrayGraph(const phoeg::Graph & g, int n, int m, graph *array)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (edge(i, j, g).second)
            {
                ADDONEEDGE(array, i, j, m);
            }
        }
    }
}

/**
 * Add the edges of the graph stored in array into the
 * graph g
 * @param g a graph with same order as array
 * @param n the order of the graph
 * @param m the size of the graph
 * @param array the graph to convert
 */
void fillGraph(phoeg::Graph& g, int n, int m, graph* array)
{
    set *row;
    //We skip the first vertex since we want the inferior
    //half of the adjacency matrix
    for (int i = 1; i < n ; ++i)
    {
        row = GRAPHROW(array, i, m);
        for (int j = 0; j < i; ++j)
        {
            if (ISELEMENT(row, j))
            {
                add_edge(i, j, g);
            }
        }
    }
}

/**
 * Fills the arrays order and ptn used by nauty in its computation of
 * the automorphism group to specify fixed vertices.
 * The order array will be filled to provide an order of the vertices
 * (the fixed ones first and the other ones after, both in order of
 * their indices).
 * The format used is that vertices of same colour are consecutive in
 * the order. Then, in ptn, each of these vertices are assigned a 1
 * except for the last one that has 0. e.g., if we have the partition
 * (2)(4)(1 3 5) (we can see that 2 and 4 are fixed), the order will be
 * [2, 4, 1, 3, 5] and ptn will be [0, 0, 1, 1, 0].
 * @param fixed the set of fixed vertices
 * @param n the size of order and ptn
 * @param order an array that will be filled to define the order of the
 * vertices
 * @param ptn a pre-colouring of the vertices
 */
void fillOrderPtn(const std::set<int> &fixed, int n, int order[], int ptn[])
{
    //place in order for fixed vertices
    int fixedPlace = 0;
    //place in order for non fixed vertices
    int orderPlace = fixed.size();
    auto fixedVert = fixed.begin();
    for (int i = 0; i < n; i++)
    {
        if ((fixedPlace < fixed.size() && i < *fixedVert) || fixedPlace >= fixed.size())
        {
            order[orderPlace] = i; //==i
            ptn[orderPlace] = 1;
            ++orderPlace;
        }
        else
        {
            order[fixedPlace] = i;
            ptn[fixedPlace] = 0;
            ++fixedPlace;
            ++fixedVert;
        }
    }
    ptn[n - 1] = 0;
}
} //namespace detail

/**
 * Fills the array orbit with the orbits of the automorphism group of the
 * graph. The format is the following :
 * For a vertex i, the ith cell of the array contains the lowest index of
 * the vertices in the orbit of that vertex. e.g., if the graph has orbits
 * (1 3)(2)(4 5), the array will be [1, 2, 1, 4, 4].
 * The set fixed is the set of vertices that are fixed and cannot be moved
 * to produce an automorphism.
 * @param g the graph
 * @param orbits an array of length n (order of g)
 * @param fixed a set (can be empty) of fixed vertices
 * @return the canonical form of the graph
 */
void cannonFormOrbit(const phoeg::Graph& g, int orbit[], const
                     std::set<int>& fixed)
{
    int n = num_vertices(g), m = num_edges(g);
    int ptn[n], order[n];
    detail::fillOrderPtn(fixed, n, order, ptn);
    if (m > 0)
    {
        static DEFAULTOPTIONS_GRAPH(options);
        options.defaultptn = 0;
        statsblk stats;
        graph array[n * m];
        EMPTYGRAPH(array, m, n);
        detail::fillArrayGraph(g, n, m, array);
        densenauty(array, order, ptn, orbit, &options, &stats, m, n, NULL);
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            orbit[i] = -1;
        }
        int minnofixed = 0;
        for (auto u : fixed)
        {
            orbit[u] = u;
            if (u == minnofixed)
            {
                minnofixed++;
            }
        }
        for (int i = 0; i < n; i++)
        {
            if (orbit[i] < 0)
            {
                orbit[i] = minnofixed;
            }
        }
    }
}

/**
 * Returns the canonical form of the given graph with n vertices and m edges
 * The new ordering of the nodes is contained in the order array given as a
 * parameter. This array should have a length equals to the order of the
 * graph.
 * @param g the graph
 * @param order an array of length n (order of g)
 * @return the canonical form of the graph
 */
phoeg::Graph cannonFormOrder(const phoeg::Graph& g, int order[])
{
    int n = num_vertices(g), m = num_edges(g);
    if (m > 0)
    {
        int ptn[n], orbit[n];
        static DEFAULTOPTIONS_GRAPH(options);
        options.getcanon = 1;
        statsblk stats;
        graph array[n * m], canon[n * m];
        EMPTYGRAPH(array, m, n);
        detail::fillArrayGraph(g, n, m, array);
        densenauty(array, order, ptn, orbit, &options, &stats, m, n, canon);
        phoeg::Graph ng(n);
        detail::fillGraph(ng, n, m, canon);
        return ng;
    }
    else
    {
        //Nauty cannot work with n > min(2000000000,64*m)
        for (int i = 0; i < n; ++i)
        {
            order[i] = i;
        }
        return phoeg::Graph(g);
    }
}

/**
 * Returns the canonical form of the given graph with n vertices and m edges
 * @param g the graph
 * @return the canonical form of the graph
 */
phoeg::Graph cannonForm(const phoeg::Graph& g)
{
    int n = num_vertices(g);
    int order[n];
    return cannonFormOrder(g, order);
}
}

#endif
