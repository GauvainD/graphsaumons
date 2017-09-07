#ifndef VERTEX_ITERATOR_HPP
#define VERTEX_ITERATOR_HPP

#include <iterator>
#include <set>
#include <utility>
#include "nauty_interface.hpp"
#include "invariants.hpp"

namespace phoeg {

    class VertexIterator : public std::iterator<std::input_iterator_tag,
        phoeg::vertex_iter> {
    private:
        phoeg::vertex_iter start, end, cur;

        std::vector<int> orbits;

        //fixed vertices should only be fixed once and in order (better
        //complexity to initialize parameters to nauty)
        std::set<int> fixed;

        //position in the orbits
        int pos;

        /**
         * Computes the orbits of the automorphism group of g with the fixed
         * vertices.
         */
        void computeOrbits(const Graph & g)
        {
            int n = num_vertices(g);
            int orbitarray[n];
            cannonFormOrbit(g, orbitarray, fixed);
            int min = -1;
            for (int i = 0; i < n; i++) {
                if (orbitarray[i] > min) {
                    min = orbitarray[i];
                    orbits.push_back(min);
                }
            }
            if (orbits.size() > pos) {
                cur = start + orbits[pos];
            }
            else {
                cur = end;
            }
        }

    public:

        /**
         * Constructor taking a graph and a set of fixed vertices.
         */
        VertexIterator(const Graph & g, const std::set<int> &fixed): pos(0),
            fixed(fixed)
        {
            auto pair = vertices(g);
            start = pair.first;
            end = pair.second;
            computeOrbits(g);
        }

        /**
         * Constructor using an empty set of fixed vertices.
         */
        VertexIterator(const Graph & g): VertexIterator(g, std::set<int>())
        {
        }

        /**
         * Iterator over the orbits of the graph with all the vertices from
         * other fixed (its fixed ones and its current one)
         */
        VertexIterator(const Graph & g, const VertexIterator &other):
            fixed(other.fixed), start(other.start), end(other.end), pos(0)
        {
            //Fixing the current vertex of the other iterator
            bool newfixed = fixed.insert(other.orbits[other.pos]).second;
            //No need to recompute if it was already fixed
            if (newfixed) {
                computeOrbits(g);
            }
        }

        VertexIterator(const VertexIterator &other): start(other.start),
            end(other.end), orbits(other.orbits), pos(other.pos), cur(other.cur)
        {
        }

        VertexIterator& operator=(const VertexIterator& other)
        {
            start = other.start;
            end = other.end;
            orbits = other.orbits;
            pos = other.pos;
            cur = other.cur;
            return *this;
        }

        VertexIterator& operator++()
        {
            ++pos;
            if (orbits.size() > pos) {
                cur = start + orbits[pos];
            }
            else {
                cur = end;
            }
        }

        VertexIterator operator++(int)
        {
            VertexIterator tmp(*this);
            operator++();
            return tmp;
        }

        bool operator==(const VertexIterator& other)
        {
            return cur == other.cur;
        }

        bool operator!=(const VertexIterator& other)
        {
            return !operator==(other);
        }

        bool operator<(const VertexIterator& other)
        {
            return cur < other.cur;
        }

        bool operator>(const VertexIterator& other)
        {
            return cur > other.cur;
        }

        bool operator<=(const VertexIterator& other)
        {
            return !(*this > other);
        }

        bool operator>=(const VertexIterator& other)
        {
            return !(*this < other);
        }

        phoeg::vertex operator*()
        {
            return *cur;
        }

        /**
         * Return true if we get past the last element
         */
        bool last()
        {
            return cur == end;
        }

    };

} // namespace phoeg


#endif
