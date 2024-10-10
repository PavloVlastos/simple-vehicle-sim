/*
 * planner.c
 */

/*
 * Includes
 */
#include "planner.h"

/*
 * Defines
 */
#define NUM_TEST_WPS 5

/*
 * Module-level variables
 */
static PLAN_payload_t path;
static planner_t planner;
static float wp_test_path[NUM_TEST_WPS][DIM2] = {
    {0.0, 0.0}, {1.0, 2.0}, {10.0, 4.0}, {-1.0, 4.0}, {-10.0, -10.0}};

static uint8_t packed_map[MAP_DFLT_NUM_BYTES_PER_MAP]; /* Internal occupancy map
                                                          (packed) */

/*
 * Private helper function prototypes
 */

int set_plan_type(planner_t p);
float gen_rand_float(float min, float max);

/*
 * Function implementations
 */

int planner_init(int verbose, planner_t p) {
    int status = ERROR;

    srand(42);

    if (verbose == 1) {
        printf(" |____ initializing planner\r\n");
    }
    if (set_plan_type(p) == SUCCESS) {
        status = SUCCESS;
    }
    if ((verbose == 1) && (status == SUCCESS)) {
        printf(" |________ planner type: %d, %s\r\n", p,
               planner_get_plan_str());
    }

    return status;
}

int planner_plan(planner_t p, const map_t *map, float new_wp[DIM2]) {
    int status = ERROR;

    switch (p) {
    case P_STATIC:
        status = SUCCESS;
        break;

    case P_MYOPIC:
        status = planner_myopic(map, new_wp);
        break;

    case P_CUSTOM:
        status = SUCCESS;
        break;

    case P_TEST:
        status = planner_test(map, new_wp);
        break;

    default:
        status = ERROR;
        break;
    }

    return status;
}

int planner_myopic(const map_t *map, float new_wp[DIM2]) {

    new_wp[0] = gen_rand_float(map->x_min, map->x_max);
    new_wp[1] = gen_rand_float(map->y_min, map->y_max);

    return SUCCESS;
}

int planner_custom(const map_t *map, float new_wp[DIM2]) {
    // int i = 0;
    // int j = 0;

    // for (j = 0; j < MAP_DFLT_Y_LEN; j++) {
    //     for (i = 0; i < MAP_DFLT_X_LEN; i++) {
    //     }
    // }
    return SUCCESS;
}

int planner_test(const map_t *map, float new_wp[DIM2]) {
    static int i_wp = 0;

    int i = 0;

    for (i = 0; i < DIM2; i++) {
        new_wp[i] = wp_test_path[i_wp][i];
    }

    i_wp++;
    i_wp %= NUM_TEST_WPS;

    return SUCCESS;
}

const char *planner_get_plan_str(void) {
    switch (planner) {
    case P_STATIC:
        return "P_STATIC";
        break;
    case P_MYOPIC:
        return "P_MYOPIC";
        break;
    case P_CUSTOM:
        return "P_CUSTOM";
        break;
    default:
        return "ERROR";
        break;
    }
}

/*
 * Private helper function implementations
 */

int set_plan_type(planner_t p) {
    int status = ERROR;

    switch (p) {
    case P_STATIC:
        status = SUCCESS;
        break;
    case P_MYOPIC:
        status = SUCCESS;
        break;
    case P_CUSTOM:
        status = SUCCESS;
        break;
    default:
        status = ERROR;
        break;
    }

    if (status == SUCCESS) {
        planner = p;
    }

    return status;
}

float gen_rand_float(float min, float max) {
    return (max - min) * ((float)rand() / RAND_MAX) + min;
}