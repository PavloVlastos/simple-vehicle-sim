/*
 * plan.hpp
 */
#ifndef PLAN_H
#define PLAN_H


/*
* Includes
*/
#include "common.h"
#include "map.h"

/*
* Defines
*/
#define PLAN_SUCCESS 0
#define PLAN_FAILURE -1

#define MAX_NUM_WP 10

/* Waypoint */
// union
// {
//     float x;
//     float y;
//     uint8_t wp_array[WP_2D * sizeof(float)];
// } waypoint;

/* TODO: Get rid of this eventually */
typedef struct
{
    uint16_t nrow;
    uint16_t ncol;
    uint8_t divisionsPerCell;
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


enum planner_e {
    P_STATIC = 0,
    P_MYOPIC,
    P_CUSTOM
};

typedef enum planner_e planner_t;

/*
 * Function prototypes
 */


int planner_init(int verbose, planner_t p);

int planner_custom(map_t map, test_t t);


#endif /* PLAN_H */
