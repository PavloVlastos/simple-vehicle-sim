/*
 * plan.cpp
 */

#include "plan.hpp"

/*
 * Module-level variables
 */
static PLAN_payload_t path;

static uint8_t unpacked[Y_LEN][X_LEN] = {0x00};

/*
 * Function implementations
 */
int plan_init(int verbose);

int plan_update(float pose[DIM3], float target_wp_out[DIM2]);

int plan_plan(float pose[DIM3], float target_wp[DIM2], SLAM_OccupancyMap_Payload_t map, PLAN_payload_t *path_out);

int plan_unpack_map(SLAM_OccupancyMap_Payload_t map) {
    
}