#include <stdio.h>
#include <string.h>

#include "common.h"
#include "controller.h"
#include "interface.h"
#include "model.h"

int main(int argc, char *argv[])
{

    int i = 0;
    uint8_t verbose = 0;
    uint8_t tcp_synch_flag = 0;
    int max_step_num = 0; /* Zero indicates no limit */
    float dt = 0.01;
    float kp = 1.0;
    float ki = 0.0;
    float kd = 0.01;
    float spd = 5.0;

    for (i = 0; i < argc; i++)
    {

        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
        {
            verbose = 1;
        }
        else if (strcmp(argv[i], "-t") == 0 ||
                 strcmp(argv[i], "--tcp-synch-flag") == 0)
        {
            tcp_synch_flag = 1;
        }
        else if (strcmp(argv[i], "--dt") == 0)
        {
            if (i + 1 < argc) /* Check for next arg*/
            {
                i++;
                dt = atof(argv[i]);
            }
        }
        else if (strcmp(argv[i], "--kp") == 0)
        {
            if (i + 1 < argc) /* Check for next arg*/
            {
                i++;
                kp = atof(argv[i]);
            }
        }
        else if (strcmp(argv[i], "--ki") == 0)
        {
            if (i + 1 < argc) /* Check for next arg*/
            {
                i++;
                ki = atof(argv[i]);
            }
        }
        else if (strcmp(argv[i], "--kd") == 0)
        {
            if (i + 1 < argc) /* Check for next arg*/
            {
                i++;
                kd = atof(argv[i]);
            }
        }
        else if (strcmp(argv[i], "-m") == 0 ||
                 strcmp(argv[i], "--max-steps") == 0)
        {
            if (i + 1 < argc) /* Check for next arg*/
            {
                i++;
                max_step_num = atoi(argv[i]);
            }
        }

        else if (strcmp(argv[i], "--speed") == 0)
        {
            if (i + 1 < argc) /* Check for next arg*/
            {
                i++;
                spd = atof(argv[i]);
            }
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            printf("optional arguments:");
            printf("    -h, --help\r\n");
            printf("    -m, --max-steps       Max number of steps for simulation (0 means no limit)\r\n");
            printf("    -t, --tcp-synch-flag, For use with python visualize_sim.py\r\n");
            printf("    --dt,                 Time step duration for simulation\r\n");
            printf("    --kp,                 Proportional gain for controller\r\n");
            printf("    --speed,              Vehicle speed, constant (m/s)\r\n");
        } else {
            printf("Invalid argument(s)! try -h or --help to see valid arguments");
        }
        if (verbose == 1)
        {
            printf("Argument: %d: %s\r\n", i, argv[i]);
        }
    }

    if (verbose == 1)
    {
        printf("Initializing mav-sim...\r\n");
        printf("    verbose        = %d\r\n", verbose);
        printf("    tcp_synch_flag = %d\r\n", tcp_synch_flag);
        printf("    kp             = %f\r\n", kp);
        printf("    ki             = %f\r\n", ki);
        printf("    kd             = %f\r\n", kd);
        printf("    vehicle speed  = %f\r\n", spd);
    }
    float t = 0.0;
    int count = 0;

    float rud_ang = 0.0;
    float target_wp[DIM2];

    uint8_t data_new;
    uint8_t data_old;

    int status = SUCCESS;

    state_t uav;

    uav.x = -10.0;
    uav.y = -10.0;
    uav.spd = spd;
    uav.psi = 0.25 * M_PI;

    controller_init();
    model_init(&uav);

    int socket = ERROR;

    if (tcp_synch_flag == 1)
    {
        socket = interface_open_tcp_connection("127.0.0.1", 9200);

        if (socket < 0)
        {
            printf("Failed to connect to server\r\n");
            return ERROR;
        }
    }

    /* Main loop */
    while (1)
    {
        /*
         * Interface
         * Only step if a byte is received.
         */
        if (tcp_synch_flag == 1)
        {

            status = interface_receive_byte(socket, &data_new);

            /* Block */
            while (data_new == data_old)
            {

                status = interface_receive_byte(socket, &data_new);

                if (status < 0)
                {
                    break;
                }
            };
            // if (verbose == 1)
            // {
            //     printf("data = 0x%02x\r\n", data_new);
            // }

            interface_send_tcp_message(socket, 0x78, uav.x);
            interface_send_tcp_message(socket, 0x79, uav.y);
            interface_send_tcp_message(socket, 0x7A, uav.psi);

            data_old = data_new;
        }

        /* Controller logic here */
        update_waypoint(uav.x, uav.y);
        get_next_waypoint(target_wp);
        rud_ang = controller_update(uav.x, uav.y, uav.psi, target_wp, kp);

        model_update(dt, rud_ang, 0.0, 0.0);
        model_get_state(&uav);

        count++;

        t += dt;

        /* Print and/or save data */
        printf("[t=%03.3f]: count=%d, x=%03.3f, y=%03.3f, psi=%03.3f, u=%03.3f, twp_x=%03.3f, twp_y=%03.3f\r\n", 
            count, t, uav.x, uav.y, uav.psi, rud_ang, target_wp[0],  target_wp[1]);

        /* remove this eventually */
        if ((count >= max_step_num) && (max_step_num != 0))
        {
            if (verbose == 1)
            {
                printf("Maximum step count reached %d/%d\r\n", count, max_step_num);
            }
            break;
        }
    }

    if (tcp_synch_flag == 1)
    {
        interface_close_tcp_connection(socket);
    }

    return ERROR;
}