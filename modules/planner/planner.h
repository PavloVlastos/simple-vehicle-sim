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
typedef struct {
    uint16_t nrow;
    uint16_t ncol;
    uint8_t divisionsPerCell;
    int8_t minXRealWorldValue;
    int8_t minYRealWorldValue;
    int8_t maxXRealWorldValue;
    int8_t maxYRealWorldValue;
    uint8_t packedOccupancyMap[MAP_PACKED_SIZE];
} SLAM_OccupancyMap_Payload_t;

typedef struct {
    float path[MAX_NUM_WP][DIM2];
} PLAN_payload_t;

enum planner_e { P_STATIC = 0, P_MYOPIC, P_CUSTOM };

typedef enum planner_e planner_t;

/*
 * Function prototypes
 */

/**
 * @param[in] verbose
 * @param[in] p The planner type
 * @return 1 for success or 0 for error
 */
int planner_init(int verbose, planner_t p);

/**
 * @param[in] p The planner type
 * @param[in] map A pointer to a map struct
 * @param[out] new_wp The next waypoint decided by the planner
 */
int planner_plan(planner_t p, const map_t *map, float new_wp[DIM2]);

/**
 * @param[in] map
 * @param[out] new_wp The next waypoint decided by the planner
 * @return 1 for success or 0 for error
 */
int planner_myopic(const map_t *map, float new_wp[DIM2]);

/**
 * @param[in] map
 * @param[out] new_wp The next waypoint decided by the planner
 * @return 1 for success or 0 for error
 */
int planner_custom(const map_t *map, float new_wp[DIM2]);

/**
 * @param p The planner type
 * @return A pointer to the planner string (const char *)
 */
const char *planner_get_plan_str(void);

#endif /* PLAN_H */
