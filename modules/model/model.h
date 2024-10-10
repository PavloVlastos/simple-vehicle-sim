#ifndef MODEL_H
#define MODEL_H

#include "common.h"
#include "lin_alg.h"
#include <math.h>

#define STATE_LEN DIM5

typedef union
{
    struct
    {
        float x;
        float y;
        float spd;
        float psi;
        float psi_rate;
    };
    float vector[STATE_LEN];
} state_t;

int model_init(const state_t *state_in);

/**
* @param[in] dt The sample-time of the model
* @param[in] rudder_angle The rudder angle of the model
* @param[in] speed The speed of the model
* @param[in] vx_dist The velocity disturbance in the x-axis
* @param[in] vy_dist The velocity disturbance in the y-axis
 */
int model_update(const float dt, float rudder_angle, float speed, float vx_dist,
                 float vy_dist);

/**
 * @param[in] angle The heading angle of the vehicle
 * @return The wrapped angle between [-pi, pi]
 */
float model_wrap_angle(float angle);

/**
 * @param[out] state_out The state of the vehicle model
 * @return 0 for success, or -1 for error
 */
int model_get_state(state_t *state_out);

/**
 * @param[out] position_out
 * @return 0 for success, or -1 for error
 */
int model_get_position(float position_out[DIM2]);

/**
 *
 */
int model_set_state(const state_t *state_in);

/**
 *
 */
int model_set_x(float x);

/**
 *
 */
int model_set_y(float y);

/**
 *
 */
int model_set_vx(float vx);

/**
 *
 */
int model_set_vy(float vy);

/**
 *
 */
int model_set_spd(float spd);

/**
 *
 */
int model_set_psi(float psi);

/**
 *
 */
int model_set_psi_rate(float psi_rate);

/**
 *
 */

#endif /* MODEL_H */
