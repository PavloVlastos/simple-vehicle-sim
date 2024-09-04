/*
 * map.hpp
 */
#ifndef MAP_H
#define MAP_H

/*
 * #includes
 */
#include "common.h"

/*
 * #defines
 */

/* TODO: Get rid of this eventually */
#define MAP_DFLT_X_MIN (float)(-5.0)
#define MAP_DFLT_X_MAX (float)(5.0)
#define MAP_DFLT_Y_MIN (float)(-5.0)
#define MAP_DFLT_Y_MAX (float)(5.0)
#define MAP_DFLT_DIV_PER_CELL (float)(10.0)
#define MAP_DFLT_RESOLUTION (1.0 / MAP_DFLT_DIV_PER_CELL)
#define MAP_DFLT_X_LEN                                                         \
    (int)((MAP_DFLT_X_MAX - MAP_DFLT_X_MIN) * MAP_DFLT_DIV_PER_CELL)
#define MAP_DFLT_Y_LEN                                                         \
    (int)((MAP_DFLT_Y_MAX - MAP_DFLT_Y_MIN) * MAP_DFLT_DIV_PER_CELL)
#define MAP_DFLT_NUM_CELL (int)(MAP_DFLT_X_LEN * MAP_DFLT_Y_LEN)
#define MAP_DFLT_BYTE_PER_CELL (1.0 / 8.0)
#define MAP_DFLT_NUM_BYTES_PER_MAP                                             \
    (int)(MAP_DFLT_X_LEN * MAP_DFLT_Y_LEN * MAP_DFLT_DIV_PER_CELL *            \
          MAP_DFLT_BYTE_PER_CELL)
#define WP_2D 2

#define MAP_PACKED_SIZE MAP_DFLT_NUM_BYTES_PER_MAP

/*
 * Module datatypes
 */

typedef struct {
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    float div_per_cell;
    float resolution;
    int x_len;
    int y_len;
    int num_cells;
    float num_bytes_per_cell;
    int num_bytes_per_map;
} map_t;

/*
 * Function prototypes
 */

/**
 * @param[in] x_min
 * @param[in] x_max
 * @param[in] y_min
 * @param[in] y_max
 * @param[in] div_per_cell
 * @return 0 for success, or -1 for error
 */
int map_init(float x_min, float x_max, float y_min, float y_max,
             float div_per_cell);
/**
 * @param[in] file_path The path and file name of the packed-map file
 * @param[out] map
 * @return 0 for success, or -1 for error
 */
int map_load_packed_map_file(const char *file_path,
                             uint8_t map[MAP_DFLT_NUM_BYTES_PER_MAP]);
/**
 * @param[in] file_path The path and file name of the packed-map file
 * @param[in] map
 * @return 0 for success, or -1 for error
 */
int map_save_packed_map_file(const char *file_path,
                             uint8_t map[MAP_DFLT_NUM_BYTES_PER_MAP]);

/**
 * @param[out] value A pointer to the value at map indices (i, j) 
 * @param[in] i
 * @param[in] j
 * @param[in] map The packed map
 * @return 0 for success, or -1 for error
 */
int map_read_cell_in_packed_map(int *value, int i, int j,
                                uint8_t map[MAP_DFLT_NUM_BYTES_PER_MAP]);
/**
 * @param[in] value The bit value (-1, 0, or 1) to write to the packed map
 * @param[in] i
 * @param[in] j
 * @param[out] map The packed map
 * @return 0 for success, or -1 for error
 */
int map_write_cell_in_packed_map(int value, int i, int j,
                                 uint8_t map[MAP_DFLT_NUM_BYTES_PER_MAP]);

#endif /* MAP_H */