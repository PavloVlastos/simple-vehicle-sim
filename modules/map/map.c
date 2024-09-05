/*
 * map.c
 */

/*
 * Includes
 */
#include "map.h"

/*
 * Private module-level variables
 */
static map_t map;

/*
 * Private helper function prototypes
 */
/**
 * @param[in] b The byte to read a bit from
 * @param[in] shift The number of times to shift a bit within the interval of
 * [1, 8]
 */
uint8_t bit_get(uint8_t b, uint8_t shift);

/**
 * @param[out] b A pointer to a byte
 * @param[in] shift The number of times to shift a bit within the interval of
 * [1, 8]
 * @param[in] value The value to set, either 0 or 1
 */
int bit_set(uint8_t *b, uint8_t shift, uint8_t value);

/*
 * Function implementations
 */

int map_init(float x_min, float x_max, float y_min, float y_max,
             float div_per_cell) {
    int status = SUCCESS;

    map.x_min = x_min;
    map.x_max = x_max;
    map.y_min = y_min;
    map.y_max = y_max;
    map.div_per_cell = div_per_cell;

    if (div_per_cell == 0.0) {
        return ERROR;
    }
    map.resolution = 1.0 / div_per_cell;

    map.x_len = (int)((x_max - x_min) * div_per_cell);
    map.y_len = (int)((y_max - y_min) * div_per_cell);

    map.num_cells = (int)(map.x_len * map.y_len);
    map.num_bytes_per_cell = (1.0 / 8.0);
    map.num_bytes_per_map =
        (map.num_cells * map.div_per_cell * map.num_bytes_per_cell);

    return status;
}

/*
 * Function implementations
 */
int map_load_packed_map_file(const char *file_path,
                             uint8_t map[MAP_DFLT_NUM_BYTES_PER_MAP]) {
    /*
     * Read in packed-map file for tests
     */
    FILE *file = fopen(file_path, "rb");

    if (file == NULL) {
        printf("Failed to open file\r\n");
        return ERROR;
    }

    size_t n_bytes_read = fread(map, 1, sizeof(map), file);

    if (n_bytes_read != sizeof(map)) {
        printf("map size = %lu bytes, but number of bytes read = %d\r\n",
               sizeof(map), (int)n_bytes_read);
        fclose(file);
        return ERROR;
    }

    fclose(file);

    return SUCCESS;
}

int map_read_cell_in_packed_map(int *value, int i, int j,
                                uint8_t map_bytes[MAP_DFLT_NUM_BYTES_PER_MAP]) {
    int i_flat = 0;
    int k = 0;
    int h = 0;
    uint8_t b = 0;
    int observed_status = 0;
    int occupied_status = 0;

    i_flat = (i * map.x_len) + j;
    h = (int)(floor((float)i_flat / 4.0));
    k = (i_flat % 4) * 2;

    b = map_bytes[h];

    observed_status = bit_get(b, 8 - k);
    occupied_status = bit_get(b, 8 - k - 1);
    *value = 0;

    if (observed_status == 0) {
        *value = -1;

    } else if ((observed_status == 1) && (occupied_status == 0)) {
        *value = 0;

    } else if ((observed_status == 1) && (occupied_status == 1)) {
        *value = 1;

    } else {
        return ERROR;
    }

    return SUCCESS;
}

int map_write_cell_in_packed_map(
    int value, int i, int j, uint8_t map_bytes[MAP_DFLT_NUM_BYTES_PER_MAP]) {
    int status = ERROR;
    int i_flat = 0;
    int k = 0;
    int h = 0;
    uint8_t b = 0;

    i_flat = (i * map.x_len) + j;
    h = (int)(floor((float)i_flat / 4.0));
    k = (i_flat % 4) * 2;

    if (value == -1) {
        status = bit_set(&b, (8 - k), 0); /* Unobserved */
        if (status == ERROR) {
            return status;
        }
        status = bit_set(&b, (8 - k - 1), 0); /* Not applicable */
        if (status == ERROR) {
            return status;
        }

    } else if (value == 0) {
        status = bit_set(&b, (8 - k), 1); /* Observed */
        if (status == ERROR) {
            return status;
        }
        status = bit_set(&b, (8 - k - 1), 0); /* Empty */
        if (status == ERROR) {
            return status;
        }

    } else if (value == 1) {
        status = bit_set(&b, (8 - k), 1); /* Observed */
        if (status == ERROR) {
            return status;
        }
        status = bit_set(&b, (8 - k - 1), 1); /* Occupied */
        if (status == ERROR) {
            return status;
        }
    }

    map_bytes[h] = b;

    return status;
}

const map_t *map_get_map(void) { return &map; }

float map_get_x_min(void) { return map.x_min; }

float map_get_x_max(void) { return map.x_max; }

float map_get_y_min(void) { return map.y_min; }

float map_get_y_max(void) { return map.y_max; }

/*
 * Private helper function implementations
 */

uint8_t bit_get(uint8_t b, uint8_t shift) {
    return (uint8_t)(b >> (shift - 1)) & (0x01);
}

int bit_set(uint8_t *b, uint8_t shift, uint8_t value) {
    if ((shift < 1) || (8 < shift)) {
        return ERROR;
    }

    /* Clear the bit */
    *b &= ~(0b00000001 << (shift - 1));

    if (value == 0) {
        /* Bit is already cleared */
        return SUCCESS;

    } else if (value == 1) {
        *b |= (0b00000001 << (shift - 1));
        return SUCCESS;

    } else {
        return ERROR;
    }

    return SUCCESS;
}