/*
 * File name: node.hpp
 *
 *  Description:
 *      This is a node class for a 2D graph for applying search algorithms
 *      like A*. Each node has a row and column index associated with a
 *      graph that has a grid structure. 
 *
 */
#ifndef NODE_H
#define NODE_H

/*
 * Includes
 */

/*
 * Defines
 */
#define MAX_NUM_NEIGHBORS 8

/*
 * node class for graph nodes for A* search algorithm
 */
class node
{
private:
    // uint64_t id;
    // uint64_t id_predecessor;
    int32_t row;
    int32_t col;
    float cost;
    node *neighbors[MAX_NUM_NEIGHBORS];

public:
    /*
     * Constructor
     */

    node(int32_t i, int32_t j, float c) : row(i), col(j), cost(c)
    {

        /* Set all neighbors for */
        for (int i = 0; i < MAX_NUM_NEIGHBORS; ++i)
        {
            neighbors[i] = nullptr;
        }
    }

    float get_row() const
    {
        return row;
    }

    float get_col() const
    {
        return col;
    }

    float get_cost() const
    {
        return cost;
    }

    // uint64_t get_id() const
    // {
    //     return id;
    // }

    // uint64_t get_id_predecessor() const
    // {
    //     return id_predecessor;
    // }
};

/*
 * Function prototypes
 */
bool compare_node(const node &a, const node &b);

#endif /* NODE_H */