/*
 * File: model.c
 */

/*
 * #includes
 */

#include "model.h"
#include <math.h>

/*
 * #defines
 */
#define TWO_PI (float) (2.0*M_PI)

/*
 * Module-level variables
 */

/*
 * Function implementations
 */

int model_init(const state_t *state_in, state_t *state) {
    int status;

    status = model_set_state(state_in, state);

    return status;
}

int model_update(state_t *state, const float dt, float rudder_angle, float speed, float vx_dist,
                 float vy_dist) {
    float base_length = 1.0;
    /* Clip rudder angle values */
    if (rudder_angle > MAX_RUD_ANG) {
        rudder_angle = MAX_RUD_ANG;
    }

    if (rudder_angle < MIN_RUD_ANG) {
        rudder_angle = MIN_RUD_ANG;
    }

    if ((BASE_LENGTH == 0.0) || (base_length == 0.0)) {
        base_length = BASE_LENGTH;
        printf("error in model_update(): BASE_LENGTH = 0.0\r\n");
        return ERROR;
    }

    if (speed > MAX_ABS_SPD) {
        speed = MAX_ABS_SPD;
        printf("warning in model_update(): speed > (MAX_ABS_SPD=%f)\r\n",
               MAX_ABS_SPD);
    }

    state->spd = speed;

    /*
     * Psi is the angle between the global y-axis and vehicle velocity vector,
     * hence sin and cos correspond to x and y respectively below.
     */
    state->x += (state->spd * sin(state->psi) + vx_dist) * dt;
    state->y += (state->spd * cos(state->psi) + vy_dist) * dt;

    state->psi_dot = 0.0;
    state->psi_dot = (-1.0) * state->spd * tan(rudder_angle) / base_length;
    state->psi += state->psi_dot * dt;

    /*
     * Wrap angle
     */
    state->psi = model_wrap_angle(state->psi);

    return SUCCESS;
}

float model_wrap_angle(float angle) {

    angle = fmod( angle + M_PI, TWO_PI);

    if (angle < 0) {
        angle += TWO_PI;
    }

    return angle - M_PI;

}

int model_get_position(float position_out[DIM2], state_t *state) {
    position_out[0] = state->x;
    position_out[1] = state->y;
    return SUCCESS;
}

int model_set_state(const state_t *state_in, state_t *state) {
    int i;

    /* Copy the state pointer information into the internal state */
    for (i = 0; i < STATE_LEN; i++) {
        state->vector[i] = state_in->vector[i];
    }
    return SUCCESS;
}

int model_set_x(float x, state_t *state) {
    state->x = x;
    return SUCCESS;
}

int model_set_y(float y, state_t *state) {
    state->y = y;
    return SUCCESS;
}

int model_set_spd(float spd, state_t *state) {
    state->spd = spd;
    return SUCCESS;
}

int model_set_psi(float psi, state_t *state) {
    state->psi = psi;
    return SUCCESS;
}