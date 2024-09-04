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
map_t map_i;

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
 * @param[out] *b A pointer to a byte
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

    map_i.x_min = x_min;
    map_i.x_max = x_max;
    map_i.y_min = y_min;
    map_i.y_max = y_max;
    map_i.div_per_cell = div_per_cell;

    if (div_per_cell == 0.0) {
        return ERROR;
    }
    map_i.resolution = 1.0 / div_per_cell;

    map_i.x_len = (int)((x_max - x_min) * div_per_cell);
    map_i.y_len = (int)((y_max - y_min) * div_per_cell);

    map_i.num_cells = (int)(map_i.x_len * map_i.y_len);
    map_i.num_bytes_per_cell = (1.0 / 8.0);
    map_i.num_bytes_per_map =
        (map_i.num_cells * map_i.div_per_cell * map_i.num_bytes_per_cel);

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

    if (file == NULL) {
        printf("Failed to open file\r\n");
        return ERROR;
    }

    size_t n_bytes_read = fread(map 1, sizeof(map), file);

    if (n_bytes_read != sizeof(map)) {
        printf("map size = %d bytes, but number of bytes read = \r\n",
               sizeof(map), n_bytes_read);
        fclose(file);
        return ERROR;
    }

    fclose(file);

    return SUCCESS;
}

int map_read_cell_in_packed_map(int i, int j,
                                uint8_t map[MAP_DFLT_NUM_BYTES_PER_MAP]);

int map_write_cell_in_packed_map(int value, int i, int j,
                                 uint8_t map[MAP_DFLT_NUM_BYTES_PER_MAP]) {
    int status = ERROR;
    int i_flat = 0;
    int k = 0;
    int h = 0;
    uint8_t b = 0;

    i_flat = (i * map_i.x_len) + j;
    h = (int)(floor(i_flat / 4));
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

    } else if (value == 0) {
        status = bit_set(&b, (8 - k), 1); /* Observed */
        if (status == ERROR) {
            return status;
        }
        status = bit_set(&b, (8 - k - 1), 1); /* Occupied */
        if (status == ERROR) {
            return status;
        }
    }

    map[h] = b;

    return status;
}

/*
 * Private helper function implementations
 */

uint8_t bit_get(uint8_t b, uint8_t shift) {
    return (uint8_t)(b >> (shift - 1)) & (0x01);
}

int bit_set(uint8_t *b, uint8_t shift, uint8_t value) {
    if ((shift < 1) || (8 < shift) {
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

    return SUCCESS
}