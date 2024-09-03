#include "controller.h"

/*
 * Module-level variables
 */
static controller_state_t controller_state = CS_STAYING_STILL;
static float wp_target[DIM2] = {0.0};

/*
 * Private function prototypes
 */

/**
 * @param a The angle to be confined to [-pi, pi]
 */
float confine_angle(float a);

/*
 * Function implmentations
 */

int controller_init(int verbose) {
    int status = 0;
    int i = 0;

    if (verbose == 1) {
        printf(" |____ initializing controller\r\n");
        printf(" |______ DIM4 = %d\r\n", DIM4);
        printf(" |______ DIM2 = %d\r\n", DIM2);
    }

    for (i = 0; i < DIM2; i++) {
        wp_target[i] = 0.0;
    }

    if (verbose == 1) {
        printf(" |____ finished controller init\r\n");
    }

    return status;
}

int controller_get_vehicle_prox(vehicle_position[DIM2]) {
    int is_near = VEHICLE_OUT_PROX;
    int i;
    float result[DIM2] = {0.0};

    /*
     * Subtract the position vector fro the taret waypont vector
     */
    for (i = 0; i < DIM2; i++) {
        result[i] = vehicle_position[i] - wp_target[i];
    }

    /*
     * Check vehicle proximity to waypoint
     */
    if (lin_alg_v_norm(result) < WP_ENTR_THRESH) {
        is_near = VEHICLE_IN_PROX;
    }

    if (lin_alg_v_norm(result) > WP_EXIT_THRESH) {
        is_near = VEHICLE_OUT_PROX;
    }

    return is_near;
}

int controller_get_target_waypoint(float wp_out[DIM2]) {
    wp_out[0] = wp_target[0];
    wp_out[1] = wp_target[1];
    return SUCCESS;
}

int controller_update(float kp, float psi, int blocked_status,
                      float vehicle_position[DIM2], float target_waypoint[DIM2],
                      float output[DIM2]) {
    int status;
    float steering = 0.0;
    float throttle = 0.0;
    float x = 0.0;
    float y = 0.0;
    float x_ref = target_waypoint[0];
    float y_ref = target_waypoint[1];

    x = vehicle_position[0];
    y = vehicle_position[1];

    /*
     * State machine
     */
    switch (controller_state) {
    case CS_STAYING_STILL:
        steering = 0.0;
        throttle = 0.0;

        /* Transition Conditions */
        if (controller_get_vehicle_prox() == VEHICLE_IN_PROX) {
            controller_state = CS_STAYING_STILL;
        }

        if ((controller_get_vehicle_prox() == VEHICLE_OUT_PROX) &&
            (blocked_status == IS_NOT_BLOCKED)) {
            controller_state = CS_MOVING_FORWARD;
        }

        break;

    case CS_MOVING_FORWARD:
        steering = controller_step_steering(x, x_ref, y, y_ref, psi, kp);\
        throttle = 1.0;

        /* Transition Conditions */
        if (controller_get_vehicle_prox() == VEHICLE_IN_PROX) {
            controller_state = CS_STAYING_STILL;
        }

        if (blocked_status == IS_BLOCKED) {
            controller_state = CS_MOVING_BACKWARD;
        }

        break;

    case CS_MOVING_BACKWARD:
        steering = controller_step_steering(x, x_ref, y, y_ref, psi, kp);
        throttle = -0.2;

        /* Transition Conditions */
        if (blocked_status == IS_BLOCKED) {
            controller_state = CS_MOVING_BACKWARD;
        }

        if (blocked_status == IS_NOT_BLOCKED) {
            controller_state = CS_MOVING_FORWARD;
        }

        break;

    default:
    }

    /*
     * Controller outputs
     */
    output[0] = steering;
    output[1] = throttle;

    return SUCCESS;
}

float controller_step_steering(float x, float x_ref, float y, float y_ref,
                               float psi, float kp) {
    float angle_target = 0.0;
    float angle_error = 0.0;
    float angle_rudder = 0.0;

    // angle_target = atan2f(y_ref - y, x_ref - x);
    angle_target = atan2f(x_ref - x, y_ref - y);

    angle_error = confine_angle(angle_target - psi);

    angle_rudder = kp * angle_error;

    /* Clip rudder angle values */
    if (angle_rudder > MAX_RUD_ANGLE) {
        angle_rudder = MAX_RUD_ANGLE;
    }

    if (angle_rudder < MIN_RUD_ANGLE) {
        angle_rudder = MIN_RUD_ANGLE;
    }

    return (-1.0) * angle_rudder;
}

/*
 * Private helper function implementations
 */
float confine_angle(float a) {
    while (a > M_PI)
        a -= 2 * M_PI;
    while (a < -M_PI)
        a += 2 * M_PI;
    return a;
}