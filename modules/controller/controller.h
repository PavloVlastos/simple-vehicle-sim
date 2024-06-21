#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "common.h"
#include "lin_alg.h"

#define WP_THRESH 1.0

int controller_init(int verbose);

int update_waypoint(float x, float y);

int get_next_waypoint(float wp_out[DIM2]);

float confine_angle(float a);

float controller_update(float x, float y, float psi, float wp[DIM2], float kp);

#endif /* CONTROLLER_H */ 
