#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        return -1;
    }

    const char *server_ip = argv[1];  // Server IP address from the command line
    int port = atoi(argv[2]);         // Convert port argument to integer

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char result[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attach socket to the provided port
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(port);   // Use the specified port

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, server_ip, &address.sin_addr) <= 0) {
        printf("Invalid address or Address not supported\n");
        return -1;
    }

    // Bind the socket to the provided IP address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    //printf("Server listening on IP %s, port %d\n", server_ip, port);

    while (1) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        // Clear the buffers
        memset(buffer, 0, BUFFER_SIZE);
        memset(result, 0, BUFFER_SIZE);

        // Read the command sent by the client
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread > 0) {
            buffer[valread] = '\0';  // Null-terminate the received string
            //printf("Command received: %s\n", buffer);

            // Open a pipe to execute the command and capture both stdout and stderr
            FILE *fp = popen(buffer, "r");
            if (fp == NULL) {
                strcpy(result, "Failed to run command\n");
            } else {
                // Read the command output or error and store it in result
                fread(result, sizeof(char), BUFFER_SIZE, fp);

                // If result is empty, send a command not found message
                if (strlen(result) == 0) {
                    strcpy(result, "Command not found or no output\n");
                }

                pclose(fp);
            }
        } else {
            strcpy(result, "No command received\n");
        }

        // Send the result (either output or error) back to the client
        send(new_socket, result, strlen(result), 0);
        //printf("Output sent to client\n");

        // Close the socket for this connection
        close(new_socket);
    }

    return 0;
}
