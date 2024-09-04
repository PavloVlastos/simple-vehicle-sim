#include "map.h"

int main(void) {
    int num_tests_pass = 0;
    int num_tests = 0;

    uint8_t map[MAP_DFLT_NUM_BYTES_PER_MAP];
    printf("Beginning UT_map.c unit tests...\r\n");

    /**************************************************************************/
    /*
     * Unit tests start here
     */
    // {
    //     const char *file_name = "random_map_0";
    // }

    {
        printf("Unit Test %d: ... ", num_tests);
        int result_obtained = 0;
        int result_expected = 1;
        int i = 0;
        int j = 0;
        int status = SUCCESS;

        status = map_write_cell_in_packed_map(result_expected, i, j, map);
        if (status == ERROR) {
            printf("    error: map_write_cell_in_packed_map()\r\n");
        }
        status = map_read_cell_in_packed_map(&result_obtained, i, j, map);
        if (status == ERROR) {
            printf("    error: map_read_cell_in_packed_map()\r\n");
        }

        if (result_obtained == result_expected) {
            printf("passed\r\n");
            num_tests_pass++;

        } else {
            printf("failed\r\n");
            printf("expected %d, got %d\r\n", result_expected, result_obtained);
        }
        num_tests++;
    }

    {
        printf("Unit Test %d: ... ", num_tests);
        int result_obtained = 0;
        int result_expected = -1;
        int i = 10;
        int j = 10;
        int status = SUCCESS;

        status = map_write_cell_in_packed_map(result_expected, i, j, map);
        if (status == ERROR) {
            printf("    error: map_write_cell_in_packed_map()\r\n");
        }
        status = map_read_cell_in_packed_map(&result_obtained, i, j, map);
        if (status == ERROR) {
            printf("    error: map_read_cell_in_packed_map()\r\n");
        }

        if (result_obtained == result_expected) {
            printf("passed\r\n");
            num_tests_pass++;

        } else {
            printf("failed\r\n");
            printf("expected %d, got %d\r\n", result_expected, result_obtained);
        }
        num_tests++;
    }

    printf("%d/%d Tests passed\r\n", num_tests_pass, num_tests);

    return SUCCESS;
}
