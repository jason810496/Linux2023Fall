// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#include "color.h"

const char START_CONNECT[] = "START_CONNECT";
const char START[] = "START";
int DEMO = 0;

void ServerConnectHint(){
    if( DEMO ){
        printf(GREEN "Connect to server!\n" RESET);
    }
}

void ServerReceiveHint(char *buffer){
    if( DEMO ){
        printf("%sServer:%s%s\n",BLUE,RESET,buffer);
        return;
    }
    char *match = strstr(buffer, START);
    if (match != NULL) {
        return;
    }
    printf("%s\n", buffer);
}

void UserInputHint(){
    if( DEMO ){
        printf( GREEN "Enter a message to send to the server: " RESET);
    }
}


int main(int argc, char const* argv[])
{
    if( argc == 2 ){
        if( strcmp(argv[1],"demo") == 0 ){
            DEMO = 1;
        }
    }

	int status, val_read, client_fd;
	struct sockaddr_in serv_addr;
	char* hello = "Hello from client";
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status
		= connect(client_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}

	send(client_fd, START_CONNECT, strlen(START_CONNECT), 0);
	
    ServerConnectHint();

    while ((val_read = read(client_fd, buffer, 1024 - 1)) > 0) {
        buffer[val_read] = '\0';
        ServerReceiveHint(buffer);

        UserInputHint();
        char input[1024];
        // read until newline
        fgets(input, 1024, stdin);
        send(client_fd, input, strlen(input), 0);
        memset(buffer, 0, 1024);
    }

	// closing the connected socket
	close(client_fd);
	return 0;
}
