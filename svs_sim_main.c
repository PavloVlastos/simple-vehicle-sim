#include <stdio.h>
#include <string.h>

#include "common.h"
#include "controller.h"
#include "interface.h"
#include "lin_alg.h"
#include "model.h"

/*
 * Helper function protypes
 */

/*
 *
 */
int main(int argc, char *argv[]) {

    uint8_t tcp_synch_flag = 0;
    int max_step_num = 1;
    int verbose = 0 float dt = 0.01;
    float kp = 1.0;
    // float ki = 0.0;
    // float kd = 0.01;
    float spd = 5.0;
    float position[DIM2] = {0.0};
    float controller_output[DIM2] = {0.0};

    float t = 0.0;
    int count = 0;

    float rud_ang = 0.0;
    float throttle = 0.0;
    float target_wp[DIM2];

    uint8_t data_new = 0;
    uint8_t data_old = 0;

    int status = SUCCESS;

    parse_args(argc, argv);

    verbose = parse_args_is_verbose();

    if (verbose == 1) {
        printf(" | Initializing svs-sim...\r\n");
        printf(" |__ verbose        = %d\r\n", verbose);
        printf(" |__ tcp_synch_flag = %d\r\n", tcp_synch_flag);
        printf(" |__ kp             = %f\r\n", kp);
        // printf(" |__ ki             = %f\r\n", ki);
        // printf(" |__ kd             = %f\r\n", kd);
        printf(" |__ vehicle speed  = %f\r\n", spd);
    }

    /**************************************************************************/
    if (verbose == 1) {
        printf(" | Initializing Simple Vehicle Simulation (SVS)...\r\n");
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
        printf("| Beginning main simulation loop... \r\n");
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

            interface_send_tcp_message(socket, 0x78, svs.x);
            interface_send_tcp_message(socket, 0x79, svs.y);
            interface_send_tcp_message(socket, 0x7A, svs.psi);

            data_old = data_new;
        }

        /*
         * Get new vehicle state information
         */
        model_get_position(position);

        /*
         * Controller logic here
         */
        controller_update(kp, svs.psi, position, target_wp, controller_output);
        rud_ang = controller_output[0];

        // update_waypoint(svs.x, svs.y);
        // get_next_waypoint(target_wp);
        // rud_ang = controller_update(svs.x, svs.y, svs.psi, target_wp, kp);

        /*
         * Update model
         */
        model_update(dt, rud_ang, 0.0, 0.0);
        model_get_state(&svs);

        count++;

        t += dt;

        /*
         * Print and/or save data
         */
        printf("[t=%03.3f]: count=%d, x=%03.3f, y=%03.3f, psi=%03.3f, "
               "u=%03.3f, twp_x=%03.3f, twp_y=%03.3f\r\n",
               t, count, svs.x, svs.y, svs.psi, rud_ang, target_wp[0],
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
