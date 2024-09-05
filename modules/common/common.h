#ifndef COMMON_H
#define COMMON_H

/*
 * #includes
 */

#include <math.h>   /* For tanf(), sinf(), cosf(), etc. */
#include <stdint.h> /* Using for uint8_t and similar */
#include <stdio.h>  /* For printf() and similar*/
#include <stdlib.h> /* Using for atoi() and atof() */
#include <string.h> /* Using for strncmp */

/*
 * #defines
 */

#define ERROR -1
#define SUCCESS 0
#define IS_BLOCKED 1
#define IS_NOT_BLOCKED 0

#define DIM2 2
#define DIM3 3
#define DIM4 4
#define DIM5 5
#define DIM6 6

#define BASE_LENGTH (float)1.0           /* vehicle base length in meters */
#define MIN_RUD_ANG (float)(-0.2 * M_PI) /* minimum rudder angle in radians */
#define MAX_RUD_ANG (float)(0.2 * M_PI)  /* maximum rudder angle in radians  */
#define MAX_ABS_SPD (float)(7.0)         /* maximum vehicle speed in m/s */

/*
 * Datatypes
 */
typedef struct {
    int x;
} test_t;

/*
 *
 */

#endif /* COMMON_H */
