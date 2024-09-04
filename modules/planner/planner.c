/*
 * planner.c
 */

/*
 * Includes
 */
#include "planner.h"
#include "common.h"

/*
 * Defines
 */

/*
 * Module-level variables
 */
static PLAN_payload_t path;
static planner_t planner;

static uint8_t packed_map[MAP_DFLT_NUM_BYTES_PER_MAP]; /* Internal occupancy map
                                                          (packed) */

/*
 * Private helper function prototypes
 */

int check_plan_type(planner_t p);
const char *get_plan_type_str(planner_t p);

/*
 * Function implementations
 */

int planner_init(int verbose, planner_t p) {
    int status = ERROR;

    if (verbose == 1) {
        printf(" |____ initializing planner\r\n");
    }
    if (set_plan_type(p) == SUCCESS) {
        status = SUCCESS;
    }
    if ((verbose == 1) && (status == SUCCESS)) {
        printf(" |________ planner type: %d, %s", p, get_plan_type_str(p));
    }

    return status;
}

int planner_custom(map_t map) {
    int i = 0;
    int j = 0;

    // for (j = 0; j < MAP_DFLT_Y_LEN; j++) {
    //     for (i = 0; i < MAP_DFLT_X_LEN; i++) {
    //     }
    // }
    return SUCCESS;
}

/*
 * Private helper function implementations
 */
int set_plan_type(planner_t p) {
    int status = ERROR;

    switch (p) {
    case P_STATIC:
        status = SUCCESS;
    case P_MYOPIC:
        status = SUCCESS;
    case P_CUSTOM:
        status = SUCCESS;
    default:
        status = ERROR;
    }

    if (status == SUCCESS) {
        planner = p;
    }

    return status;
}

const char *get_plan_type_str(planner_t planner) {
    switch (planner) {
    case P_STATIC:
        return "P_STATIC";
    case P_MYOPIC:
        return "P_MYOPIC";
    case P_CUSTOM:
        return "P_CUSTOM";
    default:
        return "ERROR";
    }
}