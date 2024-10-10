#include "interface.h"

static uint8_t large_message[MAP_DFLT_NUM_BYTES_PER_MAP + 1] = {0};

int interface_open_tcp_connection(const char *ip, int port) {
    int sock;
    struct sockaddr_in server_address;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return ERROR; // Socket creation failed
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

int interface_send_tcp_message(int sock, uint8_t id, float value) {
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

int interface_send_tcp_map(int sock, uint8_t id,
                           float map_bytes[MAP_DFLT_NUM_BYTES_PER_MAP]) {
    int i = 0;

    for (i = 0; i < (MAP_DFLT_NUM_BYTES_PER_MAP + 1); i++) {
        large_message[i] = 0; /* Clear the current byte */

        if (i == 0) {
            large_message[i] = id;
        } else {
            large_message[i] = map_bytes[i - 1];
        }
    }

    /* Send large message */
    if (send(sock, large_message, sizeof(large_message), 0) < 0) {
        return ERROR; // Send failed
    }

    return SUCCESS;
}

int interface_receive_byte(int sock, uint8_t verbose, int timeout_sec,
                           uint8_t *data) {
    ssize_t bytes_received;
    struct timeval timeout;

    // Set a timeout for receiving data
    timeout.tv_sec = timeout_sec; // Timeout in seconds
    timeout.tv_usec = 0;          // 0 microseconds (no partial seconds)

    // Apply the timeout to the socket
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout,
                   sizeof(timeout)) < 0) {
        if (verbose == 1) {
            printf(" |__ Interface: Failed to set socket timeout\r\n");
        }
        return ERROR;
    }

    // Receive a single byte from the server
    bytes_received = recv(sock, data, 1, 0);
    if (bytes_received <= 0) {
        if (verbose == 1) {
            if (bytes_received == 0) {
                printf(" |__ Interface: Connection closed by peer\r\n");
            } else if (errno == EWOULDBLOCK || errno == EAGAIN) {
                printf(" |__ Interface: Receive timeout\r\n");
            } else {
                printf(" |__ Interface: Receive failed\r\n");
            }
        }
        return ERROR;
    }

    return SUCCESS;
}

void interface_close_tcp_connection(int sock) { close(sock); }