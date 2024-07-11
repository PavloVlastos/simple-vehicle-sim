/*
 * File name: node.cpp
 *
 *  Description:
 *      This is a node class for a 2D graph for applying search algorithms
 *      like A*. Each node has a row and column index associated with a
 *      graph that has a grid structure. 
 *
 */

/*
* Includes
*/
#include "node.hpp"


bool compare_node(const node &a, const node &b)
{
    return a.get_id() > b.get_id(); // Custom comparison based on node id
}