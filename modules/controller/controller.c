#include "controller.h"

/*
 * Module-level variables
 */
static controller_state_t controller_state = CS_WAITING_FOR_NEXT_WP;

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

    if (verbose == 1) {
        printf(" |____ finished controller init\r\n");
    }

    return status;
}

int controller_get_vehicle_prox(float vehicle_position[DIM2],
                                float wp_target[DIM2]) {
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

int controller_update(float kp, float psi, int blocked_status,
                      float vehicle_position[DIM2], float target_waypoint[DIM2],
                      float output[DIM2]) {
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
    case CS_WAITING_FOR_NEXT_WP:
        steering = 0.0;
        throttle = 0.0;

        /* Transition Conditions */
        if (controller_get_vehicle_prox(vehicle_position, target_waypoint) ==
            VEHICLE_IN_PROX) {
            controller_state = CS_WAITING_FOR_NEXT_WP;
        }

        if ((controller_get_vehicle_prox(vehicle_position, target_waypoint) ==
             VEHICLE_OUT_PROX) &&
            (blocked_status == IS_NOT_BLOCKED)) {
            controller_state = CS_MOVING_FORWARD;
        }

        break;

    case CS_MOVING_FORWARD:
        steering = controller_step_steering(x, x_ref, y, y_ref, psi, kp);
        throttle = 1.0;

        /* Transition Conditions */
        if (controller_get_vehicle_prox(vehicle_position, target_waypoint) ==
            VEHICLE_IN_PROX) {
            controller_state = CS_WAITING_FOR_NEXT_WP;
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

        break;
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
    if (angle_rudder > MAX_RUD_ANG) {
        angle_rudder = MAX_RUD_ANG;
    }

    if (angle_rudder < MIN_RUD_ANG) {
        angle_rudder = MIN_RUD_ANG;
    }

    return (-1.0) * angle_rudder;
}

controller_state_t controller_get_state(void) { return controller_state; }

const char *controller_get_state_str(void) {
    switch (controller_state) {
    case CS_WAITING_FOR_NEXT_WP:
        return "CS_WAITING_FOR_NEXT_WP";
        break;
    case CS_MOVING_FORWARD:
        return "CS_MOVING_FORWARD";
        break;
    case CS_MOVING_BACKWARD:
        return "CS_MOVING_BACKWARD";
        break;
    default:
        break;
    }

    return "ERROR";
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