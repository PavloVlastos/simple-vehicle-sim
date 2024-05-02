#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>

#include "common.h"

int interface_open_tcp_connection(const char *ip, int port);

int interface_send_tcp_message(int sock, uint8_t id, float value);

int interface_receive_byte(int sock, uint8_t *data);

void interface_close_tcp_connection(int sock);

#endif /* INTERFACE_H */