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

enum planner_e { P_STATIC = 0, P_MYOPIC, P_CUSTOM, P_TEST };

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
