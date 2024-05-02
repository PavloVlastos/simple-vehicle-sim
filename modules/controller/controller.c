#include "controller.h"

static float waypoint[DIM4][DIM2];
static int wp_index = 0;

int controller_init(void)
{
    waypoint[0][0] = 0.0;   // x
    waypoint[0][1] = 10.0; // y

    waypoint[1][0] = 10.0; // x
    waypoint[1][1] = 0.0;  // y

    waypoint[2][0] = 10.0; // x
    waypoint[2][1] = -5.0; // y

    waypoint[3][0] = -10.0; // x
    waypoint[3][1] = -10.0; // y
}

int update_waypoint(float x, float y)
{
    int update_flag = 0;
    int i;
    float position[DIM2] = {x, y};
    float result[DIM2] = {0};

    for (i = 0; i < DIM2; i++)
    {
        result[i] = position[i] - waypoint[wp_index][i];
    }

    if (lin_alg_v_norm(result) < WP_THRESH)
    {
        wp_index++;
        wp_index %= DIM4;
        update_flag = 1;
    }

    return update_flag;
}

int get_next_waypoint(float wp_out[DIM2])
{
    wp_out[0] = waypoint[wp_index][0];
    wp_out[1] = waypoint[wp_index][1];
    return SUCCESS;
}

float confine_angle(float a)
{
    while (a > M_PI)
        a -= 2 * M_PI;
    while (a < -M_PI)
        a += 2 * M_PI;
    return a;
}

float controller_update(float x, float y, float psi, float wp[DIM2], float kp)
{

    float angle_rudder = 0.0;
    float angle_target = 0.0;
    float angle_error = 0.0;
    float x_ref = wp[0];
    float y_ref = wp[1];

    // angle_target = atan2f(y_ref - y, x_ref - x);
    angle_target = atan2f(x_ref - x, y_ref - y);

    angle_error = confine_angle(angle_target - psi);

    angle_rudder = kp * angle_error;

    /* Clip rudder angle values */
    if (angle_rudder > MAX_RUD_ANGLE)
    {
        angle_rudder = MAX_RUD_ANGLE;
    }

    if (angle_rudder < MIN_RUD_ANGLE)
    {
        angle_rudder = MIN_RUD_ANGLE;
    }

    return -angle_rudder;
}
