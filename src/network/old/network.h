//lang::CwC

#pragma once

#include <stdio.h>
#include <sys/socket.h> 

class Client {
public:
	struct addrinfo info, *server;
	const char* my_ip;
	const char* server_ip = "127.0.0.1"; //hardcode localhost for now
	const char** peer_ip; 

	int send(char* msg, int socket) {
		if (connect(socket, server->ai_addr, sizeof(struct sockaddr)) == -1) {
	            perror("Connection failed\n");
	            exit(EXIT_FAILURE);
		}

	    char buffer[256];
		bzero(buffer,256);
		strcpy(buffer, msg);
	    int n = write(socket,buffer,strlen(buffer));

	    return n;
	}

	char* recv(char* msg, int socket) {
		if (connect(socket, server->ai_addr, sizeof(struct sockaddr)) == -1) {
	            perror("Connection failed\n");
	            exit(EXIT_FAILURE);
		}

	    char buffer[256];
		bzero(buffer,256);
	    bzero(buffer,256);
	    int n = read(socket,buffer,255);

	    return buffer;
	}
};


class Server {
	public:
	struct addrinfo info, *server;
	const char* server_ip;
	const char** peer_ip; 
	int customers[256]; //sockets bound to each connected client
	char** client_ips; //ips of all connections

	//send message to socket
	int send(char* msg, int socket) {
		if (connect(socket, server->ai_addr, sizeof(struct sockaddr)) == -1) {
	            perror("Connection failed\n");
	            exit(EXIT_FAILURE);
		}

	    char buffer[256];
		bzero(buffer,256);
		strcpy(buffer, msg);
	    int n = write(socket,buffer,strlen(buffer));

	    return n;
	}

	char* recv(int socket) {
		if (connect(socket, server->ai_addr, sizeof(struct sockaddr)) == -1) {
	            perror("Connection failed\n");
	            exit(EXIT_FAILURE);
		}

	    char buffer[256];
		bzero(buffer,256);
	    bzero(buffer,256);
	    int n = read(socket,buffer,255);

	    return buffer;
	}
};