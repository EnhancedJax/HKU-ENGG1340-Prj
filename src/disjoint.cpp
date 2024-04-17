/**
 * @file disjoint.cpp
 * @brief Disjoint set data structure.
 * 
 * This file contains the implementation of the disjoint set data structure. 
 * Attribution: https://github.com/138paulmiller/PyMaze/blob/master/disjointSet.py
 * Sampled logic and converted to C++ for use.
*/

#include "disjoint.hpp"
#include <unordered_map>

using namespace std;

/**
 * Creates a new set containing a single element.
 * 
 * @param v The element to create a new set with.
 */
void DisjointSet::make_set(int v)
{
    parent[v] = v;
    rank[v] = 0;
}

/**
 * Finds the representative element of the set that contains the element v.
 * 
 * @param v The element to find.
 * @return The representative element of the set that contains v.
 */
int DisjointSet::find(int v)
{
    if (v == parent[v])
    {
        return v;
    }
    // Path compression
    return parent[v] = find(parent[v]);
}

/**
 * Unions two sets by performing a union by rank operation.
 * 
 * @param a The representative element of the first set.
 * @param b The representative element of the second set.
 */
void DisjointSet::union_sets(int a, int b)
{
    a = find(a);
    b = find(b);
    if (a != b)
    {
        // Union by rank
        if (rank[a] < rank[b])
        {
            swap(a, b);
        }
        parent[b] = a;
        if (rank[a] == rank[b])
        {
            ++rank[a];
        }
    }
}