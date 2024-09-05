#ifndef PARSE_ARGS_H
#define PARSE_ARGS_H

/*
 * #includes
 */
#include "common.h"

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
uint8_t parse_args_is_verbose();

/**
 * @return 1 if tcp_synch_flag is set, else 0
 */
uint8_t parse_args_is_tcp_synch();

/**
 * @return Maximum step number for simulation
 */
int parse_args_get_max_step_num();

/**
 * @return Speed of vehicle
 */
float parse_args_get_speed();

/**
 * @return Propotional gain for steering controller
 */
float parse_args_get_kp();

/**
 * @return Integral gain for steering controller
 */
float parse_args_get_ki();

/**
 * @return Derivative gain for steering controller
 */
float parse_args_get_kd();

/**
 * @return Sample-time for simluation
 */
float parse_args_get_dt();

#endif /* PARSE_ARGS_H */