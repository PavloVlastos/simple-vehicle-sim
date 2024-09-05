
#include "model.h"
#include <math.h>

static state_t int_st; /* Internal state */

int model_init(const state_t *state_in) {
    int status;

    status = model_set_state(state_in);

    return status;
}

int model_update(const float dt, float rudder_angle, float speed, float vx_dist,
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

    int_st.spd = speed;

    /*
     * Psi is the angle between the global y-axis and vehicle velocity vector,
     * hence sin and cos correspond to x and y respecively below.
     */
    int_st.x += (int_st.spd * sin(int_st.psi) + vx_dist) * dt;
    int_st.y += (int_st.spd * cos(int_st.psi) + vy_dist) * dt;

    int_st.psi_rate = 0.0;
    int_st.psi_rate = (-1.0) * int_st.spd * tan(rudder_angle) / base_length;
    int_st.psi += int_st.psi_rate * dt;

    return SUCCESS;
}

int model_get_state(state_t *state_out) {
    int i;

    /* Copy the internal state information into the state pointer */
    for (i = 0; i < STATE_LEN; i++) {
        state_out->vector[i] = int_st.vector[i];
    }

    return SUCCESS;
}

int model_get_position(float position_out[DIM2]) {
    position_out[0] = int_st.x;
    position_out[0] = int_st.y;
    return SUCCESS;
}

int model_set_state(const state_t *state_in) {
    int i;

    /* Copy the state pointer information into the internal state */
    for (i = 0; i < STATE_LEN; i++) {
        int_st.vector[i] = state_in->vector[i];
    }
    return SUCCESS;
}

int model_set_x(float x) {
    int_st.x = x;
    return SUCCESS;
}

int model_set_y(float y) {
    int_st.y = y;
    return SUCCESS;
}

int model_set_spd(float spd) {
    int_st.spd = spd;
    return SUCCESS;
}

int model_set_psi(float psi) {
    int_st.psi = psi;
    return SUCCESS;
}