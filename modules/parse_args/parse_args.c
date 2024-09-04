#include "parse_args.h"

/*
 * Module-level variables
 */
static int verbose = 0;

/*
 * Function implmentations
 */
int parse_args(int argc, char *argv[]) {
    int i = 0;
    for (i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-t") == 0 ||
                   strcmp(argv[i], "--tcp-synch-flag") == 0) {
            tcp_synch_flag = 1;
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

                if (max_step_num <= 0) {
                    printf("Error: maximum step number cannot less than or "
                           "equal to zero!\r\n");
                    return ERROR;
                }
            }
        }

        else if (strcmp(argv[i], "--speed") == 0) {
            if (i + 1 < argc) /* Check for next arg*/
            {
                i++;
                spd = atof(argv[i]);
            }
        }
        // else if (strcmp(argv[i], "--map") == 0)
        // {
        //     if (i + 1 < argc) /* Check for next arg*/
        //     {
        //         i++;
        //         FILE *file_ptr;
        //         const char *filename = argv[i];

        //         // Open the binary file for reading
        //         file_ptr = fopen(filename, "rb");
        //         if (file_ptr == NULL)
        //         {
        //             printf("ERROR: Error opening file:%s\r\n", filename);
        //             return ERROR;
        //         }

        //         n_bytes = getFileSize(filename);

        //         if (n_bytes != MAP_DFLT_NUM_BYTES_PER_MAP)
        //         {
        //             printf("ERROR: Incorrect file size:%s, expected %d, got
        //             %d\r\n",
        //                    filename, n_bytes, MAP_DFLT_NUM_BYTES_PER_MAP);
        //             return ERROR;
        //         }

        //         n_bytes = fread(packed_map, sizeof(unsigned char),
        //         MAP_DFLT_NUM_BYTES_PER_MAP, file_ptr);
        //     }
        // }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("optional arguments:\r\n");
            printf("    -h, --help\r\n");
            printf("    -m, --max-steps       Max number of steps for "
                   "simulation (0 means no limit)\r\n");
            printf("    -t, --tcp-synch-flag, For use with python "
                   "visualize_sim.py\r\n");
            printf("    --dt,                 Time step duration for "
                   "simulation\r\n");
            printf("    --kp,                 Proportional gain for "
                   "controller\r\n");
            printf(
                "    --speed,              Vehicle speed, constant (m/s)\r\n");

            return SUCCESS;
        } else {
            printf(
                "Invalid argument(s)! try -h or --help to see valid arguments");

            return ERROR;
        }

        if (verbose == 1) {
            printf("Argument: %d: %s\r\n", i, argv[i]);
        }
    }
}

int parse_args_is_verbose() { return verbose; }