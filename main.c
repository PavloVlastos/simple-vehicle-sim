#include <stdio.h>
#include <string.h>

#include "common.h"
#include "controller.h"
#include "interface_visualize.h"
#include "lin_alg.h"
#include "map.h"
#include "model.h"
#include "parse_args.h"
#include "planner.h"

/*
 * #defines
 */
#define PORT_VISUALIZE 9200
#define PORT_STRESS 9900

/*
 * Helper function prototypes
 */

/*
 *
 */
int main(int argc, char *argv[]) {
  uint8_t animate_flag = 0;
  uint8_t stress_test = 0;
  int max_step_num = 1;
  uint8_t verbose = 0;

  float dt = 0.01;
  float kp = 1.0;
  float ki = 0.0; /* @TODO: tunes this */
  float kd = 0.0; /* @TODO: tunes this */
  float spd = 5.0;

  float position[DIM2] = {0.0};
  float controller_output[DIM2] = {0.0};

  float t = 0.0;

  int count = 0;
  int t_out_sec = 1;

  int ping_count = 0;

  float steer_cmd = 0.0;
  float throttle_cmd = 0.0;
  float target_wp[DIM2] = {0.0};

  uint8_t data_new = 0;
  uint8_t data_old = 0;
  uint8_t blocked = IS_NOT_BLOCKED;
  uint8_t safe_to_integrate = 1;
  float rx_stress = 0.0;

  int status = SUCCESS;

  planner_t p = P_TEST;

  controller_state_t cs_curr = CS_WAITING_FOR_NEXT_WP;
  controller_state_t cs_last = CS_WAITING_FOR_NEXT_WP;

  /**************************************************************************/
  parse_args(argc, argv);

  verbose = parse_args_is_verbose();
  animate_flag = parse_args_is_animate();
  stress_test = parse_args_is_stress_test();
  max_step_num = parse_args_get_max_step_num();
  spd = parse_args_get_speed();
  kp = parse_args_get_kp();
  ki = parse_args_get_ki();
  kd = parse_args_get_kd();
  p = parse_args_get_plan();

  if (verbose == 1) {
    printf(" | Initializing svs-sim...\r\n");
    printf(" |__ verbose        = %d\r\n", verbose);
    printf(" |__ animate_flag   = %d\r\n", animate_flag);
    printf(" |__ stress_test    = %d\r\n", stress_test);
    printf(" |__ kp             = %f\r\n", kp);
    printf(" |__ ki             = %f\r\n", ki);
    printf(" |__ kd             = %f\r\n", kd);
    printf(" |__ kp             = %f\r\n", kp);
    printf(" |__ vehicle speed  = %f\r\n", spd);
    printf(" |__ path planner   = %d\r\n", p);
    printf(" |__ max_step_num   = %d\r\n", max_step_num);
  }

  /**************************************************************************/
  if (verbose == 1) {
    printf(" | Initializing Simple Vehicle Simulation (SVS) ...\r\n");
  }
  state_t svs;

  svs.x = MAP_DFLT_X_MIN;
  svs.y = MAP_DFLT_Y_MIN;
  svs.spd = spd;
  svs.psi = 0.25 * M_PI;

  if (verbose == 1) {
    printf(" |__ Initializing controller ...\r\n");
  }
  controller_init(verbose);

  if (verbose == 1) {
    printf(" |__ Initializing model ...\r\n");
  }
  model_init(&svs);

  if (verbose == 1) {
    printf(" |__ Initializing map ...\r\n");
  }
  map_init(MAP_DFLT_X_MIN, MAP_DFLT_X_MAX, MAP_DFLT_Y_MIN, MAP_DFLT_Y_MAX,
           MAP_DFLT_DIV_PER_CELL);

  if (verbose == 1) {
    printf(" |__ Initializing planner ...\r\n");
  }
  planner_init(verbose, p);

  int socket_animate = ERROR;
  if (animate_flag == 1) {
    if (verbose == 1) {
      printf(" |__ Initializing TCP interface_visualize for animation ...\r\n");
    }

    socket_animate =
        interface_vis_open_tcp_connection("127.0.0.1", PORT_VISUALIZE);

    if (socket_animate < 0) {
      printf("Failed to connect to server for animating\r\n");
      return ERROR;
    }
  }

  int socket_stress_test = ERROR;
  if (stress_test == 1) {
    if (verbose == 1) {
      printf(" |__ Initializing TCP interface_stress for stress test ...\r\n");
    }

    socket_stress_test =
        interface_stress_open_tcp_connection("127.0.0.1", PORT_STRESS);

    if (socket_stress_test < 0) {
      printf("Failed to connect to server for stress testing\r\n");
      return ERROR;
    }
  }

  /*
   * Finished all module initialization at this point
   */

  if (verbose == 1) {
    printf(" | SVS initialized\r\n");
  }

  if (verbose == 1) {
    printf(" | Beginning main simulation loop... \r\n");
  }

  /**************************************************************************/
  /* Main loop */
  while (1) {
    /*
     * Animation
     * Only step if a byte is received.
     */
    if (animate_flag == 1) {

      /*
       * Transmit data
       */

      interface_vis_send_tcp_message(socket_animate, MSG_STATE_X, svs.x);
      interface_vis_send_tcp_message(socket_animate, MSG_STATE_Y, svs.y);
      interface_vis_send_tcp_message(socket_animate, MSG_STATE_PSI, svs.psi);
      interface_vis_send_tcp_message(socket_animate, MSG_TARGET_X,
                                     target_wp[0]);
      interface_vis_send_tcp_message(socket_animate, MSG_TARGET_Y,
                                     target_wp[1]);

      if (verbose == 1) {
        printf(" | waiting for data byte...\r\n");
      }

      status = interface_vis_receive_byte(socket_animate, verbose, t_out_sec,
                                          &data_new);

      if (verbose == 1) {
        printf(" | (data_new, data_old) = (%d, %d)\r\n", data_new, data_old);
      }

      if (verbose == 1) {
        printf("\n | data = 0x%02x\r\n", data_new);
      }

      if (data_new != data_old) {
        safe_to_integrate = 1;
      } else {
        safe_to_integrate = 0;
      }

      data_old = data_new;
    }

    /*
     * Stress-test logic
     */
    if (stress_test == 1) {

      /*
       * Transmit data
       */
      interface_stress_send_tcp_message(socket_stress_test, MSG_STATE_PSI,
                                        svs.psi_dot);

      if (verbose == 1) {
        printf(" | waiting for input float(s) from AdaStress...\r\n");
      }

      status = interface_stress_receive_float(socket_stress_test, verbose,
                                              t_out_sec, &rx_stress);

      if (verbose == 1) {
        printf(" | rx_stress = %f, socket_stress_test = %d\r\n", rx_stress,
               socket_stress_test);
      }

      // if (verbose == 1)
      // {
      //     printf("\n | data = 0x%02x\r\n", data_new);
      // }

      // if (data_new != data_old)
      // {
      //     safe_to_integrate = 1;
      // }
      // else
      // {
      //     safe_to_integrate = 0;
      // }

      // data_old = data_new;
    }

    /*
     * Get new vehicle state information
     */
    model_get_position(position);

    /*
     * Path planner logic here
     */
    cs_curr = controller_get_state();
    if (cs_curr == CS_WAITING_FOR_NEXT_WP) {
      planner_plan(p, map_get_map(), target_wp);
    }

    if (cs_last != cs_curr) {
      cs_last = cs_curr;
      if (verbose == 1) {
        printf(" |__ New controller state = %s\r\n",
               controller_get_state_str());
      }
    }

    /*
     * Controller logic here
     */
    controller_update(kp, svs.psi, blocked, position, target_wp,
                      controller_output);
    steer_cmd = controller_output[0];
    throttle_cmd = controller_output[1];

    /*
     * Update model
     */
    if (safe_to_integrate) {
      model_update(dt, steer_cmd, svs.spd, 0.0, 0.0);
      model_get_state(&svs);

      count++;

      t += dt;

      /*
       * Print and/or save data
       */
      printf("[t=%03.3f]: count=%d, cs= %d, x=%03.3f, y=%03.3f, psi=%03.3f, "
             "psi_dot=%03.3f,"
             "steer_cmd=%03.3f, speed=%03.3f, twp_x=%03.3f, "
             "twp_y=%03.3f\r\n",
             t, count, cs_curr, svs.x, svs.y, svs.psi, svs.psi_dot, steer_cmd,
             svs.spd, target_wp[0], target_wp[1]);

      if (count >= max_step_num) {
        if (verbose == 1) {
          printf("Maximum step count reached %d/%d\r\n", count, max_step_num);
        }
        break;
      }
    }
  }

  if (animate_flag == 1) {
    interface_vis_close_tcp_connection(socket_animate);
  }

  if (stress_test == 1) {
    interface_vis_close_tcp_connection(socket_stress_test);
  }

  return status;
}

/*
 * Helper function implementations
 */
