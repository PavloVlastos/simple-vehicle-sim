#ifndef MODEL_H
#define MODEL_H

#include "common.h"
#include "lin_alg.h"
#include <math.h>

#define STATE_LEN DIM5

typedef union {
  struct {
    float x;
    float y;
    float spd;
    float psi;
    float psi_dot;
  };
  float vector[STATE_LEN];
} state_t;

int model_init(const state_t *state_in, state_t *state);

/**
 * @param[in] dt The sample-time of the model
 * @param[in] rudder_angle The rudder angle of the model
 * @param[in] speed The speed of the model
 * @param[in] vx_dist The velocity disturbance in the x-axis
 * @param[in] vy_dist The velocity disturbance in the y-axis
 */
int model_update(state_t *state, const float dt, float rudder_angle,
                 float speed, float vx_dist, float vy_dist);

/**
 * @param[in] angle The heading angle of the vehicle
 * @return The wrapped angle between [-pi, pi]
 */
float model_wrap_angle(float angle);

/**
 * @param[out] position_out
 * @return 0 for success, or -1 for error
 */
int model_get_position(float position_out[DIM2], state_t *state);

/**
 *
 */
int model_set_state(const state_t *state_in, state_t *state);

/**
 *
 */
int model_set_x(float x, state_t *state);

/**
 *
 */
int model_set_y(float y, state_t *state);

/**
 *
 */
int model_set_vx(float vx, state_t *state);

/**
 *
 */
int model_set_vy(float vy, state_t *state);

/**
 *
 */
int model_set_spd(float spd, state_t *state);

/**
 *
 */
int model_set_psi(float psi, state_t *state);

/**
 *
 */
int model_set_psi_dot(float psi_dot, state_t *state);

/**
 *
 */

#endif /* MODEL_H */
