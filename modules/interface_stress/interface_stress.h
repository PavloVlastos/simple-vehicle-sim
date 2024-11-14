#ifndef INTERFACE_STRESS_H
#define INTERFACE_STRESS_H

#include "common.h"
#include "map.h"

/**
 * @param[in] ip The IP address as a string
 * @param[in] port The port number
 */
int interface_stress_open_tcp_connection(const char *ip, int port);

/**
 * @param[in] sock The socket number
 * @param[in] id The message ID
 * @param[in] value The message value
 */
int interface_stress_send_tcp_message(int sock, uint8_t id, float value);

// /**
//  * @param[in] sock The socket number
//  * @param[in] id The message ID
//  */
// int interface_stress_send_tcp_map(int sock, uint8_t id,
//                            float map_bytes[MAP_DFLT_NUM_BYTES_PER_MAP]);

/**
 * @param[in] sock The socket number
 * @param[in] verbose A verbose flag
 * @param[in] timeout_sec The timeout in seconds
 * @param[out] data A pointer do a data byte
 */
int interface_stress_receive_byte(int sock, uint8_t verbose, int timeout_sec,
                                  uint8_t *data);

/**
 * @param[in] sock The socket number
 * @param[in] verbose A verbose flag
 * @param[in] timeout_sec The timeout in seconds
 * @param[out] data A pointer do a float
 */
int interface_stress_receive_float(int sock, uint8_t verbose, int timeout_sec,
                                   float *data);

/**
 * @param[in] sock The socket number
 */
void interface_stress_close_tcp_connection(int sock);

#endif /* INTERFACE_STRESS_H */