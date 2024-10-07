#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <server_ip> <port> <command>\n", argv[0]);
        return -1;
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);  // Convert port argument to integer
    const char *command = argv[3];

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);  // Use the specified port

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        printf("Invalid address or Address not supported\n");
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection failed\n");
        return -1;
    }

    // Send the command provided as a command line argument
    send(sock, command, strlen(command), 0);
    //printf("Command sent: %s\n", command);

    // Read the response from the server
    int valread = read(sock, buffer, BUFFER_SIZE);
    //printf("Output received from server:\n%s\n", buffer);
    printf(buffer);

    // Close the socket
    close(sock);

    return 0;
}
