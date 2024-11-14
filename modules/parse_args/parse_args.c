#include "parse_args.h"

/*
 * Module-level variables
 */
static uint8_t verbose = 0;
static uint8_t animate = 0;
static uint8_t stress_test = 0;
static int max_step_num = 0;
static float speed = 15.0;
static float kp = 1.0;
static float ki = 0.0;
static float kd = 0.01;
static float dt = 0.01;
static planner_t plan = P_STATIC;

/*
 * Function implmentations
 */
int parse_args(int argc, char *argv[]) {
  int i = 0;
  for (i = 1; i < argc; i++) {

    if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
      verbose = 1;
    } else if (strcmp(argv[i], "-a") == 0 ||
               strcmp(argv[i], "--animate") == 0) {
      animate = 1;
    } else if (strcmp(argv[i], "--dt") == 0) {
      if (i + 1 < argc) /* Check for next arg*/
      {
        i++;
        dt = atof(argv[i]);
      }
    } else if (strcmp(argv[i], "--kp") == 0) {
      if (i + 1 < argc) /* Check for next arg*/
      {
        i++;
        kp = atof(argv[i]);
      }
    } else if (strcmp(argv[i], "--ki") == 0) {
      if (i + 1 < argc) /* Check for next arg*/
      {
        i++;
        ki = atof(argv[i]);
      }
    } else if (strcmp(argv[i], "--kd") == 0) {
      if (i + 1 < argc) /* Check for next arg*/
      {
        i++;
        kd = atof(argv[i]);
      }
    } else if (strcmp(argv[i], "-m") == 0 ||
               strcmp(argv[i], "--max-steps") == 0) {
      if (i + 1 < argc) /* Check for next arg*/
      {
        i++;
        max_step_num = atoi(argv[i]);

        if (max_step_num < 0) {
          printf("Error: maximum step number cannot less than or "
                 "equal to zero!\r\n");
          return ERROR;
        }
      }
    } else if (strcmp(argv[i], "-p") == 0 ||
               strcmp(argv[i], "--planner") == 0) {
      if (i + 1 < argc) /* Check for next arg*/
      {
        i++;
        plan = (planner_t)atoi(argv[i]);

        if ((plan <= 0) || (plan > 5)) {
          printf("Error: planner type is invalid: %d\r\n", plan);
          return ERROR;
        }
      }
    }

    else if (strcmp(argv[i], "--speed") == 0) {
      if (i + 1 < argc) /* Check for next arg*/
      {
        i++;
        speed = atof(argv[i]);
      }
    } else if (strcmp(argv[i], "--stress-test") == 0) {
      stress_test = 1;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      printf("optional arguments:\r\n");
      printf("    -h, --help\r\n");
      printf("    -v, --verbose\r\n");
      printf("    -m, --max-steps    Max number of steps for "
             "simulation (0 means no limit)\r\n");
      printf("    -a, --animate, For use with python "
             "visualize_sim.py\r\n");
      printf("    --stress-test, For use with AdaStress \r\n");
      printf("    --dt,              Time step duration for "
             "simulation\r\n");
      printf("    --kp,              Proportional gain for "
             "controller\r\n");
      printf("    --ki,              Integral gain for "
             "controller\r\n");
      printf("    --kd,              Derivative gain for "
             "controller\r\n");
      printf("    --speed,           Vehicle speed, constant (m/s)\r\n");
      printf(
          "    --planner,         Path planner type, either 0, 1, 2, or 3\r\n");

      return SUCCESS;
    } else {
      printf("Invalid argument(s)! try -h or --help to see valid arguments");

      return ERROR;
    }

    if (verbose == 1) {
      printf("Argument: %d: %s\r\n", i, argv[i]);
    }
  }

  return SUCCESS;
}

uint8_t parse_args_is_verbose(void) { return verbose; }
uint8_t parse_args_is_animate(void) { return animate; }
uint8_t parse_args_is_stress_test(void) { return stress_test; }
int parse_args_get_max_step_num(void) { return max_step_num; }
float parse_args_get_speed(void) { return speed; }
float parse_args_get_kp(void) { return kp; }
float parse_args_get_ki(void) { return ki; }
float parse_args_get_kd(void) { return kd; }
float parse_args_get_dt(void) { return dt; }
planner_t parse_args_get_plan(void) { return plan; }