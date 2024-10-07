# RemoteCommand

This project consists of two C programs: a server and a client. The server listens on a specified IP address and port, accepts commands from the client, executes them, and returns the result (or an error message if the command fails). The client sends shell commands to the server and prints the server's response.

## Features
- **Server**: Accepts commands from a client, executes them, and returns the result.
- **Client**: Sends a command to the server and displays the result.
- Both the server IP address and port are configurable as command-line parameters.

## Requirements
- A Linux-based system.
- GCC (GNU Compiler Collection) installed.

## Compilation

To compile both the server and the client, follow the instructions below:

### Server

```bash
gcc server.c -o server
gcc client.c -o client

./server <server_ip> <port>
./client <server_ip> <port> <command>
