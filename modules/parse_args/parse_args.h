#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

/*
 * #includes
 */
#include "common.h"
#include "planner.h"

/*
 * Module datatypes
 */

/*
 * Function prototypes
 */

/**
 * @param[in] argc
 * @param[in] argv
 * @return 0 for success or -1 for error
 */
int parse_args(int argc, char *argv[]);

/**
 * @return 1 or 0 if verbose or not verbose
 */
uint8_t parse_args_is_verbose(void);

/**
 * @return 1 if tcp_synch_flag is set, else 0
 */
uint8_t parse_args_is_tcp_synch(void);

/**
 * @return Maximum step number for simulation
 */
int parse_args_get_max_step_num(void);

/**
 * @return Speed of vehicle
 */
float parse_args_get_speed(void);

/**
 * @return Propotional gain for steering controller
 */
float parse_args_get_kp(void);

/**
 * @return Integral gain for steering controller
 */
float parse_args_get_ki(void);

/**
 * @return Derivative gain for steering controller
 */
float parse_args_get_kd(void);

/**
 * @return Sample-time for simluation
 */
float parse_args_get_dt(void);

/**
 * @return The path planner type
 */
planner_t parse_args_get_plan(void);

#endif /* PARSE_ARGS_H */