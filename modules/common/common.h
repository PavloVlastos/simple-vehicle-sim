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

/*
 * Error/Success Codes
 */
#define ERROR -1
#define SUCCESS 0

/*
 * Blocking
 */
#define IS_BLOCKED 1
#define IS_NOT_BLOCKED 0

/*
 * Message IDs
 */
#define MSG_PING 0x42
#define MSG_STATE_X 0x70
#define MSG_STATE_Y 0x71
#define MSG_STATE_SPEED 0x72
#define MSG_STATE_PSI 0x73
#define MSG_STATE_PSI_DOT 0x74
#define MSG_TARGET_X 0x75
#define MSG_TARGET_Y 0x76
#define MSG_MAP 0x77
/*
 * Dimensions
 */
#define DIM2 2
#define DIM3 3
#define DIM4 4
#define DIM5 5
#define DIM6 6

/*
 * Vehicle specific
 */
#define BASE_LENGTH (float)1.0           /* vehicle base length in meters */
#define MIN_RUD_ANG (float)(-0.2 * M_PI) /* minimum rudder angle in radians */
#define MAX_RUD_ANG (float)(0.2 * M_PI)  /* maximum rudder angle in radians  */
#define MAX_ABS_SPD (float)(50.0)        /* maximum vehicle speed in m/s */

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
