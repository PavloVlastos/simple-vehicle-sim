#include <stdio.h>
#include <string.h>

#include "common.h"
#include "controller.h"
#include "interface.h"
#include "lin_alg.h"
#include "map.h"
#include "model.h"
#include "parse_args.h"
#include "planner.h"

/*
* #defines
*/

/*
 * Helper function prototypes
 */

/*
 *
 */
int main(int argc, char *argv[]) {
    uint8_t tcp_synch_flag = 0;
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

    float rud_ang = 0.0;
    float throttle = 0.0;
    float target_wp[DIM2] = {0.0};

    uint8_t data_new = 0;
    uint8_t data_old = 0;
    uint8_t blocked = IS_NOT_BLOCKED;

    int status = SUCCESS;

    planner_t p = P_TEST;

    controller_state_t cs_curr = CS_WAITING_FOR_NEXT_WP;
    controller_state_t cs_last = CS_WAITING_FOR_NEXT_WP;

    /**************************************************************************/
    parse_args(argc, argv);

    verbose = parse_args_is_verbose();
    tcp_synch_flag = parse_args_is_tcp_synch();
    max_step_num = parse_args_get_max_step_num();
    spd = parse_args_get_speed();
    kp = parse_args_get_kp();
    ki = parse_args_get_ki();
    kd = parse_args_get_kd();

    if (verbose == 1) {
        printf(" | Initializing svs-sim...\r\n");
        printf(" |__ verbose        = %d\r\n", verbose);
        printf(" |__ tcp_synch_flag = %d\r\n", tcp_synch_flag);
        printf(" |__ kp             = %f\r\n", kp);
        printf(" |__ ki             = %f\r\n", ki);
        printf(" |__ kd             = %f\r\n", kd);
        printf(" |__ kp             = %f\r\n", kp);
        printf(" |__ vehicle speed  = %f\r\n", spd);
        printf(" |__ max_step_num   = %d\r\n", max_step_num);
    }

    /**************************************************************************/
    if (verbose == 1) {
        printf(" | Initializing Simple Vehicle Simulation (SVS) ...\r\n");
    }
    state_t svs;

    svs.x = -10.0;
    svs.y = -10.0;
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

    if (verbose == 1) {
        printf(" | SVS initialized\r\n");
    }

    int socket = ERROR;

    if (tcp_synch_flag == 1) {
        socket = interface_open_tcp_connection("127.0.0.1", 9200);

        if (socket < 0) {
            printf("Failed to connect to server\r\n");
            return ERROR;
        }
    }

    if (verbose == 1) {
        printf(" | Beginning main simulation loop... \r\n");
    }

    /**************************************************************************/
    /* Main loop */
    while (1) {
        /*
         * Interface
         * Only step if a byte is received.
         */
        if (tcp_synch_flag == 1) {

            status = interface_receive_byte(socket, &data_new);

            /* Block */
            while (data_new == data_old) {

                status = interface_receive_byte(socket, &data_new);

                if (status < 0) {
                    break;
                }
            };
            if (verbose == 1) {
                printf("data = 0x%02x\r\n", data_new);
            }

            interface_send_tcp_message(socket, MSG_STATE_X, svs.x);
            interface_send_tcp_message(socket, MSG_STATE_Y, svs.y);
            interface_send_tcp_message(socket, MSG_STATE_PSI, svs.psi);
            interface_send_tcp_message(socket, MSG_TARGET_X, target_wp[0]);
            interface_send_tcp_message(socket, MSG_TARGET_Y, target_wp[1]);
            // interface_send_tcp_map(socket, 0x7B, map_get_map());

            data_old = data_new;
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
        rud_ang = controller_output[0];

        /*
         * Update model
         */
        model_update(dt, rud_ang, svs.spd, 0.0, 0.0);
        model_get_state(&svs);

        count++;

        t += dt;

        /*
         * Print and/or save data
         */
        printf("[t=%03.3f]: count=%d, cs= %d, x=%03.3f, y=%03.3f, psi=%03.3f, "
               "u=%03.3f, twp_x=%03.3f, twp_y=%03.3f\r\n",
               t, count, cs_curr, svs.x, svs.y, svs.psi, rud_ang, target_wp[0],
               target_wp[1]);

        if (count >= max_step_num) {
            if (verbose == 1) {
                printf("Maximum step count reached %d/%d\r\n", count,
                       max_step_num);
            }
            break;
        }
    }

    if (tcp_synch_flag == 1) {
        interface_close_tcp_connection(socket);
    }

    return status;
}

/*
 * Helper function implementations
 */
