/*
 * plan.hpp
 */
#ifndef PLAN_H
#define PLAN_H


/*
* Includes
*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "common.h"

/*
* Defines
*/
#define PLAN_SUCCESS 0
#define PLAN_FAILURE -1

#define WP_THRESH 1.0
#define MAX_NUM_WP 10

/* TODO: Get rid of this eventually */
#define MAP_DFLT_X_MIN (float)(-5.0)
#define MAP_DFLT_X_MAX (float)(5.0)
#define MAP_DFLT_Y_MIN (float)(-5.0)
#define MAP_DFLT_Y_MAX (float)(5.0)
#define MAP_DFLT_DIV_PER_CELL (float)(10.0)
#define MAP_DFLT_RESOLUTION (1.0 / MAP_DFLT_DIV_PER_CELL)
#define MAP_DFLT_X_LEN (int)((MAP_DFLT_X_MAX - MAP_DFLT_X_MIN) * MAP_DFLT_DIV_PER_CELL)
#define MAP_DFLT_Y_LEN (int)((MAP_DFLT_Y_MAX - MAP_DFLT_Y_MIN) * MAP_DFLT_DIV_PER_CELL)
#define MAP_DFLT_NUM_CELL (int)(MAP_DFLT_X_LEN * MAP_DFLT_Y_LEN)
#define MAP_DFLT_BYTE_PER_CELL (1.0/8.0)
#define MAP_DFLT_NUM_BYTES_PER_MAP (int)(MAP_DFLT_X_LEN*MAP_DFLT_Y_LEN*MAP_DFLT_DIV_PER_CELL*MAP_DFLT_BYTE_PER_CELL)
#define WP_2D 2

#define MAP_PACKED_SIZE MAP_DFLT_NUM_BYTES_PER_MAP

/* Waypoint */
union
{
    float x;
    float y;
    uint8_t wp_array[WP_2D * sizeof(float)];
} waypoint;

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
    float path[MAX_NUM_WP][DIM2];
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

/*
 *
 */
int plan_search(void);

#endif /* PLAN_H */
