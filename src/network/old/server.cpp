// lang:Cpp

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT_NO "10000"
#define BACKLOG 2

int main (int argc, char* argv[])
{
	const char* server_ip;

	if(argc > 1) {
		server_ip = argv[2];
	}
	else {
		perror("Server must have provided IP\n");
        exit(EXIT_FAILURE);
	}
	
    struct addrinfo info, *server;

    memset(&info, 0, sizeof(struct addrinfo));
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_STREAM;
	info.ai_flags = AI_PASSIVE;

	server_ip = "127.0.0.1"; //hardcode localhost for now
    getaddrinfo(server_ip, PORT_NO, &info, &server);
    if (server == NULL) {
        perror("Server address error\n");
        exit(EXIT_FAILURE);
    }

	// open  socket
	int sock = socket(server->ai_family, server->ai_socktype, server->ai_protocol);
	if (sock == -1) {
		perror("Socket fail.\n");
		exit(EXIT_FAILURE);
	}

	// bind socket to local port
	if (bind(sock, server->ai_addr, server->ai_addrlen) == -1) {
		perror("Bind fail.\n");
		exit(EXIT_FAILURE);
	}

	if (listen(sock, BACKLOG) == -1) {
		perror("Listen fail.\n");
		exit(EXIT_FAILURE);
	}

	puts("Server listening...");

	// STORE CLIENTS
	int customers[BACKLOG];
	char client_ip1[256];
	char client_ip2[256];

	int how_many = 0;
	int n;

	// Client addr
	struct sockaddr_storage client_addr;
	socklen_t client_addr_size = sizeof(client_addr);

	char buffer[256];
	// Accept all incoming connections
	for (;;) {
		if (how_many == BACKLOG) {
			break;
		}

		int client_sock = accept(sock, (struct sockaddr*)&client_addr, &client_addr_size);
		if (client_sock == -1) {
			perror("Client sock accept fail.\n");
			exit(EXIT_FAILURE);
		} 
		//break; // questionable here??

		bzero(buffer,256);
		n = read(client_sock,buffer,255);

		if (n < 0) { 
			perror("ERROR reading from socket");
			exit(EXIT_FAILURE);
		}

		//buffer[n] = '\0';

		// store client ip
		if (how_many==0) {
			customers[how_many] = client_sock;
			strcpy(client_ip1, buffer);
		printf("ip1: %s\n", client_ip1);
		}
		else {
			customers[how_many] = client_sock;
			strcpy(client_ip2, buffer);
		printf("IP2: %s\n", client_ip2);
		}


		how_many++;
		printf("%d connected\n", how_many);
		
	}

	n = write(customers[1], client_ip1, strlen(client_ip1));
	n = write(customers[0], client_ip2, strlen(client_ip2));

	return 0;

}
