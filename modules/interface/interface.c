#include "interface.h"

int interface_open_tcp_connection(const char *ip, int port)
{
    int sock;
    struct sockaddr_in server_address;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        return ERROR; // Socket creation failed
    }

    // Define the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_address.sin_addr); // Assumes success

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        close(sock);
        return ERROR; // Connection failed
    }

    return sock;
}

int interface_send_tcp_message(int sock, uint8_t id, float value)
{
    uint8_t message[5];

    // Prepare the message: ID + float value
    message[0] = id;
    memcpy(&message[1], &value, sizeof(float)); // Assumes float is 4 bytes

    // Send message
    if (send(sock, message, sizeof(message), 0) < 0)
    {
        return ERROR; // Send failed
    }

    return SUCCESS;
}
int interface_receive_byte(int sock, uint8_t *data)
{
    ssize_t bytes_received;

    // Receive a single byte from the server
    bytes_received = recv(sock, data, 1, 0);
    if (bytes_received <= 0)
    {
        printf("Interface: Receive failed\r\n");
        return ERROR;
    }

    return SUCCESS;
}

void interface_close_tcp_connection(int sock)
{
    close(sock);
}