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

int model_update(const float dt, float rudder_angle, float vx_dist,
                 float vy_dist);

int model_get_state(state_t *state_out);

int model_set_state(const state_t *state_in);

int model_set_x(float x);
int model_set_y(float y);
int model_set_vx(float vx);
int model_set_vy(float vy);
int model_set_spd(float spd);
int model_set_psi(float psi);
int model_set_psi_rate(float psi_rate);

#endif /* MODEL_H */
