/*
 * plan.hpp
 */
#ifndef PLAN_H
#define PLAN_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "lin_alg.h"

#define WP_THRESH 1.0
#define MAX_NUM_WP 10

/* TODO: Get rid of this eventually */
#define X_LEN 100
#define Y_LEN 100
#define MAP_PACKED_SIZE 2500
#define WP_2D 2

/* Waypoint */
union waypoint_u {
    float x;
    float y;
    uint8_t wp_array[WP_2D*sizeof(float)];
};

typedef waypoint_u waypoint_t;

/* TODO: Get rid of this eventually */
typedef struct
{
    uint16_t nrow;
    uint16_t ncol;
    uint8_t resolution;
    int8_t minXRealWorldValue;
    int8_t minYRealWorldValue;
    int8_t maxXRealWorldValue;
    int8_t maxYRealWorldValue;
    uint8_t packedOccupancyMap[MAP_PACKED_SIZE];
} SLAM_OccupancyMap_Payload_t;

typedef struct
{
    float path[MAX_NUM_WP][DIM3];
} PLAN_payload_t;

/*
 * Function prototypes
 */

/*
 *
 */
int plan_init(int verbose);

/*
 *
 */
int plan_update(float pose[DIM3], float target_wp_out[DIM2]);

/*
 *
 */
int plan_plan(float pose[DIM3], float target_wp[DIM2], SLAM_OccupancyMap_Payload_t map, PLAN_payload_t *path_out);

/*
*
*/
int plan_unpack_map(SLAM_OccupancyMap_Payload_t map);


#endif /* PLAN_H */
