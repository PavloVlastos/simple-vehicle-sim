#include "interface_stress.h"
#include <arpa/inet.h> // For ntohl()
#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h> // Required for struct timeval
#include <unistd.h>

static uint8_t large_message[MAP_DFLT_NUM_BYTES_PER_MAP + 1] = {0};

int interface_stress_open_tcp_connection(const char *ip, int port) {
  int sock;
  struct sockaddr_in server_address;
  int opt = 1;
  int32_t status = 0;

  // Create socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    return ERROR; // Socket creation failed
  }

  // Set SO_REUSEADDR to avoid issues with reused addresses
  status = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (status < 0) {
    perror("setsockopt failed");
    close(sock);
    return ERROR;
  }

  // Define the server address
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  inet_pton(AF_INET, ip, &server_address.sin_addr); // Assumes success

  // Connect to server
  if (connect(sock, (struct sockaddr *)&server_address,
              sizeof(server_address)) < 0) {
    close(sock);
    return ERROR; // Connection failed
  }

  return sock;
}

int interface_stress_send_tcp_message(int sock, uint8_t id, float value) {
  uint8_t message[5];

  /* Prepare the message: ID + float value */
  message[0] = id;
  memcpy(&message[1], &value, sizeof(float)); // Assumes float is 4 bytes

  /* Send message */
  if (send(sock, message, sizeof(message), 0) < 0) {
    return ERROR; // Send failed
  }

  return SUCCESS;
}

int interface_stress_receive_cmd_byte(int sock, uint8_t verbose,
                                      int timeout_sec, uint8_t *data) {
  ssize_t bytes_received;
  struct timeval timeout;
  int32_t status = 0;

  // Set a timeout for receiving data
  timeout.tv_sec = timeout_sec; // Timeout in seconds
  timeout.tv_usec = 0;          // 0 microseconds (no partial seconds)

  // Apply the timeout to the socket
  status = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout,
                      sizeof(timeout));
  if (status < 0) {
    if (verbose == 1) {
      printf(" |__ Interface_stress: Failed to set socket timeout\r\n");
    }
    return ERROR;
  }

  // Receive a single byte from the server
  bytes_received = recv(sock, data, 1, 0);
  if (bytes_received <= 0) {
    if (verbose == 1) {
      if (bytes_received == 0) {
        printf(" |__ Interface_stress: Connection closed by peer\r\n");
      } else if (errno == EWOULDBLOCK || errno == EAGAIN) {
        printf(" |__ Interface_stress: Receive timeout\r\n");
      } else {
        printf(" |__ Interface_stress: Receive failed\r\n");
      }
    }
    return ERROR;
  }

  return SUCCESS;
}

int interface_stress_receive_reset_cmd(int sock, uint8_t verbose,
                                       int timeout_sec) {
  uint8_t b = 0;

  enum cmd_parse_state_e {
    CMD_PARSE_STATE_0 = 0,
    CMD_PARSE_STATE_1,
    CMD_PARSE_STATE_2,
    CMD_PARSE_STATE_3,
    CMD_PARSE_STATE_4,
    CMD_PARSE_STATE_5,
    CMD_PARSE_STATE_6,
    CMD_PARSE_STATE_7
  };

  typedef enum cmd_parse_state_e cmd_parse_state_t;

  cmd_parse_state_t state_new = CMD_PARSE_STATE_0;
  cmd_parse_state_t state_old = CMD_PARSE_STATE_1;

  int status = ERROR;

  while (state_new != state_old) {
    state_old = state_new;
    status = interface_stress_receive_cmd_byte(sock, verbose, timeout_sec, &b);
    if (status == SUCCESS) {
      switch (state_new) {
      case CMD_PARSE_STATE_0:
        if (b == 0x01) {
          state_new = CMD_PARSE_STATE_1;
        } else {
          return SUCCESS;
        }
        break;

      case CMD_PARSE_STATE_1:
        if (b == 0x20) {
          state_new = CMD_PARSE_STATE_2;
        } else {
          return SUCCESS;
        }
        break;

      case CMD_PARSE_STATE_2:
        if (b == 0x03) {
          state_new = CMD_PARSE_STATE_3;
        } else {
          return SUCCESS;
        }
        break;

      case CMD_PARSE_STATE_3:
        if (b == 0xAA) {
          state_new = CMD_PARSE_STATE_4;
        } else {
          return SUCCESS;
        }
        break;

      case CMD_PARSE_STATE_4:
        if (b == 0xEE) {
          state_new = CMD_PARSE_STATE_5;
        } else {
          return SUCCESS;
        }
        break;

      case CMD_PARSE_STATE_5:
        if (b == 0x03) {
          state_new = CMD_PARSE_STATE_6;
        } else {
          return SUCCESS;
        }
        break;

      case CMD_PARSE_STATE_6:
        if (b == 0x20) {
          state_new = CMD_PARSE_STATE_7;
        } else {
          return SUCCESS;
        }
        break;

      case CMD_PARSE_STATE_7:
        if (b == 0x01) {
          return RESET;
        } else {
          return SUCCESS;
        }
        break;
      }
    }
  }

  return SUCCESS;
}

int interface_stress_receive_float(int sock, uint8_t verbose, int timeout_sec,
                                   float *data) {
  ssize_t bytes_received;
  struct timeval timeout;
  uint32_t int_value;
  uint8_t buffer[4];
  int32_t status = 0;

  timeout.tv_sec = timeout_sec; // Timeout in seconds
  timeout.tv_usec = 0;          // 0 microseconds (no partial seconds)

  status = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout,
                      sizeof(timeout));
  if (status != 0) {
    if (verbose) {
      printf(
          " |__ Interface_stress: Failed to set socket timeout, status: %d\r\n",
          status);
    }
    return ERROR;
  }

  bytes_received = recv(sock, buffer, 4, 0);
  if (bytes_received <= 0) {
    if (verbose) {
      if (bytes_received == 0) {
        printf(" |__ Interface_stress: Connection closed by peer, "
               "bytes_received=%d\r\n",
               bytes_received);
      } else if (errno == EWOULDBLOCK || errno == EAGAIN) {
        printf(" |__ Interface_stress: Receive timeout, bytes_received=%d\r\n",
               bytes_received);
      } else {
        printf(" |__ Interface_stress: Receive failed, bytes_received=%d\r\n",
               bytes_received);
      }
      printf(" |__ Interface_stress: recv error code (errno): %d\n", errno);
      printf(" |__ Interface_stress: recv error message: %s\n",
             strerror(errno));
    }
    return ERROR;
  }

  if (bytes_received != 4) {
    if (verbose) {
      printf(" |__ Interface_stress: Incomplete float received\r\n");
    }
    return ERROR;
  }

  memcpy(&int_value, buffer, sizeof(int_value));
  int_value = ntohl(int_value); // Convert from network byte order

  memcpy(data, &int_value, sizeof(float));

  if (fabs(*data) > MAX_RUD_ANG) {
    *data = 0;
    return ERROR;
  }

  if (verbose) {
    printf(" |__ Interface_stress: Received float: %f\r\n", *data);
  }

  return SUCCESS;
}

void interface_stress_close_tcp_connection(int sock) { close(sock); }