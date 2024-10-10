#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "lin_alg.h"

/*
 * #defines
 */
#define WP_EXIT_THRESH 0.7
#define WP_ENTR_THRESH 0.2

#define VEHICLE_IN_PROX 1
#define VEHICLE_OUT_PROX 0

/*
 * Module datatypes
 */
enum controller_states_e {
    CS_WAITING_FOR_NEXT_WP = 0,
    CS_MOVING_FORWARD,
    CS_MOVING_BACKWARD
};

typedef enum controller_states_e controller_state_t;

/*
 * Function prototypes
 */

/**
 * @param[in] verbose A print flag to help with debugging
 * @return 0 for success, or -1 for error
 */
int controller_init(int verbose);

/**
 * @param[in] vehicle_position The vehicle position [x, y]
 * @param[in] wp_target The target waypoint [wp_x, wp_y]
 * @return 0 for success, or -1 for error
 */
controller_get_vehicle_prox(float vehicle_position[DIM2],
                            float wp_target[DIM2]);

/**
 * @param[in] kp The desired proportional gain of the steering controller
 * @param[in] psi Heading angle of vehicle in radians
 * @param[in] blocked_status A flag that indicates if the vehicle is blocked or
 * not
 * @param[in] vehicle_position The vehicle position in meters
 * @param[in] target_waypoint The target waypoint in meters
 * @param[out] output The controller output. The first element is steering [-1,
 * 1] and the second element is throttle
 * @return 0 for success, or -1 for error
 */
int controller_update(float kp, float psi, int blocked_status,
                      float vehicle_position[DIM2], float target_waypoint[DIM2],
                      float output[DIM2]);

/**
 * @param[in] x Vehicle position in x-axis
 * @param[in] x_ref Waypoint x coordinate
 * @param[in] y Vehicle position in y-axis
 * @param[in] y_ref Waypoint y coordinate
 * @param[in] psi Vehicle heading angle
 * @param[in] kp Steering controller proportional gain
 * @return Steering controller output
 */
float controller_step_steering(float x, float x_ref, float y, float y_ref,
                               float psi, float kp);

/**
 * @return The state of the controller
 */
controller_state_t controller_get_state(void);

/**
 * @return The state of the controller as a string
 */
const char *controller_get_state_str(void);

#endif /* CONTROLLER_H */
