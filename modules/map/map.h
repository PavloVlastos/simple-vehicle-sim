/*
 * map.hpp
 */
#ifndef MAP_H
#define MAP_H

/*
 * #includes
 */
#include "planner.h"
#include <math.h>
#include <stdio.h>

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
 * @param[in] i
 * @param[in] j
 * @param[in] map The packed map
 * @return map value at (i, j) indices
 */
int map_read_cell_in_packed_map(int i, int j,
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