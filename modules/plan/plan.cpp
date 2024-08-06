/*
 * plan.cpp
 */

#include "plan.hpp"
#include "node.hpp"
#include <queue>
#include <cmath>

/*
 * Includes
 */

/*
 * Defines
 */

/*
 * Module-level variables
 */
static PLAN_payload_t path;

static uint8_t packed_map[MAP_DFLT_NUM_BYTES_PER_MAP]; /* Internal occupancy map (packed) */

/*
 * Data structures
 */
std::priority_queue<node, std::vector<node>, bool (*)(const node &, const node &)> open_set_pq(compare_node);

/*
 * Function implementations
 */
int plan_init(int verbose);

int plan_apply_a_star_search(node &start, node &target, float h)
{
    int status = PLAN_SUCCESS;
    open_set_pq.push(start);

    return status;
}

// float plan_heuristic(node &a, node &b) {
//     return sqrt(a->get_)
// }

int plan_update(float pose[DIM3], float target_wp_out[DIM2]);

int plan_plan(float pose[DIM3], float target_wp[DIM2], SLAM_OccupancyMap_Payload_t map, PLAN_payload_t *path_out);
