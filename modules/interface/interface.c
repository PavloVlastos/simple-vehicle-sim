#include "interface.h"
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

int interface_open_tcp_connection(const char *ip, int port)
{
    int sock;
    struct sockaddr_in server_address;
    int opt = 1;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        return ERROR; // Socket creation failed
    }

    // Set SO_REUSEADDR to avoid issues with reused addresses
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
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
                sizeof(server_address)) < 0)
    {
        close(sock);
        return ERROR; // Connection failed
    }

    return sock;
}

int interface_send_tcp_message(int sock, uint8_t id, float value)
{
    uint8_t message[5];

    /* Prepare the message: ID + float value */
    message[0] = id;
    memcpy(&message[1], &value, sizeof(float)); // Assumes float is 4 bytes

    /* Send message */
    if (send(sock, message, sizeof(message), 0) < 0)
    {
        return ERROR; // Send failed
    }

    return SUCCESS;
}

int interface_send_tcp_map(int sock, uint8_t id,
                           float map_bytes[MAP_DFLT_NUM_BYTES_PER_MAP])
{
    int i = 0;

    for (i = 0; i < (MAP_DFLT_NUM_BYTES_PER_MAP + 1); i++)
    {
        large_message[i] = 0; /* Clear the current byte */

        if (i == 0)
        {
            large_message[i] = id;
        }
        else
        {
            large_message[i] = map_bytes[i - 1];
        }
    }

    /* Send large message */
    if (send(sock, large_message, sizeof(large_message), 0) < 0)
    {
        return ERROR; // Send failed
    }

    return SUCCESS;
}

int interface_receive_byte(int sock, uint8_t verbose, int timeout_sec,
                           uint8_t *data)
{
    ssize_t bytes_received;
    struct timeval timeout;

    // Set a timeout for receiving data
    timeout.tv_sec = timeout_sec; // Timeout in seconds
    timeout.tv_usec = 0;          // 0 microseconds (no partial seconds)

    // Apply the timeout to the socket
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout,
                   sizeof(timeout)) < 0)
    {
        if (verbose == 1)
        {
            printf(" |__ Interface: Failed to set socket timeout\r\n");
        }
        return ERROR;
    }

    // Receive a single byte from the server
    bytes_received = recv(sock, data, 1, 0);
    if (bytes_received <= 0)
    {
        if (verbose == 1)
        {
            if (bytes_received == 0)
            {
                printf(" |__ Interface: Connection closed by peer\r\n");
            }
            else if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                printf(" |__ Interface: Receive timeout\r\n");
            }
            else
            {
                printf(" |__ Interface: Receive failed\r\n");
            }
        }
        return ERROR;
    }

    return SUCCESS;
}

int interface_receive_float(int sock, uint8_t verbose, int timeout_sec, 
                            float *data)
{
    ssize_t bytes_received;
    struct timeval timeout;
    uint32_t int_value;
    uint8_t buffer[4];

    timeout.tv_sec = timeout_sec; // Timeout in seconds
    timeout.tv_usec = 0;          // 0 microseconds (no partial seconds)

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) < 0)
    {
        if (verbose)
        {
            printf(" |__ Interface: Failed to set socket timeout\r\n");
        }
        return ERROR;
    }

    bytes_received = recv(sock, buffer, 4, 0);
    if (bytes_received <= 0)
    {
        if (verbose)
        {
            if (bytes_received == 0)
            {
                printf(" |__ Interface: Connection closed by peer\r\n");
            }
            else if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                printf(" |__ Interface: Receive timeout\r\n");
            }
            else
            {
                printf(" |__ Interface: Receive failed\r\n");
            }
        }
        return ERROR;
    }

    if (bytes_received != 4)
    {
        if (verbose)
        {
            printf(" |__ Interface: Incomplete float received\r\n");
        }
        return ERROR;
    }

    memcpy(&int_value, buffer, sizeof(int_value));
    int_value = ntohl(int_value); // Convert from network byte order

    memcpy(data, &int_value, sizeof(float));

    if (verbose)
    {
        printf(" |__ Interface: Received float: %f\r\n", *data);
    }

    return SUCCESS;
}

void interface_close_tcp_connection(int sock) { close(sock); }