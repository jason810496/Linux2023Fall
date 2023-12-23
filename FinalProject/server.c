#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
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

/* operations */
const char ADD[] = "add";
const char ABS[] = "abs";
const char MUL[] = "mul";
const char KILL[] = "kill";
const char HELLO[] = "Hello";
const char SPACE[] = " ";

// if `DEMO` is set to 1
// server will print out the helper message instead of `Hello`
const char INVALID[] = "Invalid command!";

int main(int argc, char const *argv[])
{
    if (argc == 2)
    {
        if (strcmp(argv[1], "demo") == 0)
        {
            DEMO = 1;
        }
    }

    int server_fd, new_socket;
    ssize_t val_read;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    // if (setsockopt(server_fd, SOL_SOCKET,
    //                SO_REUSEADDR | SO_REUSEPORT, &opt,
    //                sizeof(opt))) {

    /* On Mac OS X, SO_REUSEPORT is not a valid option. */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {

        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             &addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while ((val_read = read(new_socket, buffer, 1024 - 1)) > 0)
    {
        printf("%sClient:%s%s", BLUE, RESET, buffer);

        char output[1024] = {};
        int invalid = 1;

        // server will send START_CONNECT when first connected
        if (strcmp(buffer, START_CONNECT) == 0)
        {
            invalid = 0;
            printf(GREEN "\nConnect to client!\n" RESET);
            sprintf(output, "%s", START);
            send(new_socket, output, strlen(output), 0);
        }

        // match `add` operation
        char *match_ptr = strstr(buffer, ADD);
        if (match_ptr != NULL)
        {
            invalid = 0;
            // parse the string by space
            char *token = strtok(buffer, SPACE);
            int sum = 0;
            while (token != NULL)
            {
                token = strtok(NULL, SPACE);
                if (token != NULL)
                {
                    sum += atoi(token);
                }
            }
            // stdout to string
            sprintf(output, "%d", sum);
            send(new_socket, output, strlen(output), 0);
        }

        // match `abs` operation
        match_ptr = strstr(buffer, ABS);
        if (match_ptr != NULL)
        {
            invalid = 0;
            char *token = strtok(buffer, SPACE);
            int num = 0;
            while (token != NULL)
            {
                token = strtok(NULL, SPACE);
                if (token != NULL)
                {
                    num = atoi(token);
                }
            }
            sprintf(output, "%d", abs(num));
            send(new_socket, output, strlen(output), 0);
        }

        // match `mul` operation
        match_ptr = strstr(buffer, MUL);
        if (match_ptr != NULL)
        {
            invalid = 0;
            char *token = strtok(buffer, SPACE);
            int mul = 1;
            while (token != NULL)
            {
                token = strtok(NULL, SPACE);
                if (token != NULL)
                {
                    mul *= atoi(token);
                }
            }
            sprintf(output, "%d", mul);
            send(new_socket, output, strlen(output), 0);
        }

        // match `kill` operation
        match_ptr = strstr(buffer, KILL);
        if (match_ptr != NULL)
        {
            printf(RED "\nServer is killed!\n" RESET);
            break;
        }

        // if no match
        if (invalid)
        {
            // print out the helper message
            // if `DEMO` is set to 1
            if (DEMO)
            {
                send(new_socket, INVALID, strlen(INVALID), 0);
            }
            else
            {
                send(new_socket, HELLO, strlen(HELLO), 0);
            }
        }

        memset(buffer, 0, sizeof(buffer));
    }

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    close(server_fd);
    return 0;
}