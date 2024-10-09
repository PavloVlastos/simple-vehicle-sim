#ifndef INTERFACE_H
#define INTERFACE_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"
#include "map.h"

/**
 * @param[in] ip The IP address as a string
 * @param[in] port The port number
 */
int interface_open_tcp_connection(const char *ip, int port);

/**
 * @param[in] sock The socket number
 * @param[in] id The message ID
 * @param[in] value The message value
 */
int interface_send_tcp_message(int sock, uint8_t id, float value);

// /**
//  * @param[in] sock The socket number
//  * @param[in] id The message ID
//  */
// int interface_send_tcp_map(int sock, uint8_t id,
//                            float map_bytes[MAP_DFLT_NUM_BYTES_PER_MAP]);

/**
 * @param[in] sock The socket number
 * @param[in] verbose A verbose flag
 * @param[out] data A pointer do a data byte
 */
int interface_receive_byte(int sock, uint8_t verbose, uint8_t *data);

/**
 * @param[in] sock The socket number
 */
void interface_close_tcp_connection(int sock);

#endif /* INTERFACE_H */