// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#include "color.h"

static int DEMO = 0;

/* pre-defined messages */
// client will send START_CONNECT when first connected
const char START_CONNECT[] = "START_CONNECT";
// server will send START when first connected
const char START[] = "START";

void ServerConnectHint()
{
    if (DEMO)
    {
        printf(GREEN "Connect to server!\n" RESET);
    }
}

void ServerReceiveHint(char *buffer)
{
    if (DEMO)
    {
        printf("%sServer:%s%s\n", BLUE, RESET, buffer);
        return;
    }

    // server will send START when first connected
    char *match = strstr(buffer, START);
    if (match != NULL)
    {
        return;
    }
    printf("%s\n", buffer);
}

void UserInputHint()
{
    if (DEMO)
    {
        printf(GREEN "Enter a message to send to the server: " RESET);
    }
}

int main(int argc, char const *argv[])
{
    // if `demo` is passed in as an argument
    // client will print out the helper message
    if (argc == 2)
    {
        if (strcmp(argv[1], "demo") == 0)
        {
            DEMO = 1;
        }
    }

    // socket connection from geeksforgeeks
    int status, val_read, client_fd;
    struct sockaddr_in serv_addr;

    char buffer[1024] = {0};
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    // client will send START_CONNECT when first connected
    send(client_fd, START_CONNECT, strlen(START_CONNECT), 0);
    ServerConnectHint();

    // main logic of the client
    while ((val_read = read(client_fd, buffer, 1024 - 1)) > 0)
    {
        buffer[val_read] = '\0';
        // helper function to print out the server's message
        // if `DEMO` is set to 1, then it will print out the helper message
        ServerReceiveHint(buffer);

        // helper function to print out the user input hint ( if `DEMO` is set to 1)
        UserInputHint();
        char input[1024];
        // read until newline
        fgets(input, 1024, stdin);
        send(client_fd, input, strlen(input), 0);

        // clear the buffer
        memset(buffer, 0, 1024);
    }

    // closing the connected socket
    close(client_fd);
    return 0;
}
