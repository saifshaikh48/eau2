/// lang:Cpp

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT_NO "10000"


int main (int argc, char* argv[])
{
	// Init addr vars
	struct addrinfo info, *server;
	const char* my_ip = "127.0.0.1";
	const char* server_ip = "127.0.0.1"; //hardcode localhost for now
	const char* peer_ip; 

//	if(argc > 1) {
//		my_ip = argv[2];
//	}
//	else {
//		perror("Client must have provided IP\n");
//        exit(EXIT_FAILURE);
//	}

	// Formalities..
    memset(&info, 0, sizeof(struct addrinfo));
    info.ai_family = AF_INET;
    info.ai_socktype = SOCK_STREAM;

    // TODO Complete address specification given IP and port number.
    getaddrinfo(server_ip, PORT_NO, &info, &server);
    if (server == NULL) {
            perror("Server address error\n");
            exit(EXIT_FAILURE);
    }

	// Establish sock
    int s = socket(server->ai_family, server->ai_socktype, 0);
    if (s == -1) {
            perror("Socket malfunction\n");
            exit(EXIT_FAILURE);
    }

    // Connect the socket and establish the pipeline. 
    if (connect(s, server->ai_addr, sizeof(struct sockaddr)) == -1) {
            perror("Connection failed\n");
            exit(EXIT_FAILURE);
	}

	// TODO
	//send server my_ip, listen to server and read response for peer_ip
    char buffer[256];
	bzero(buffer,256);
	strcpy(buffer, my_ip);
    int n = write(s,buffer,strlen(buffer));

    if (n < 0) {
		perror("ERROR writing to socket");
		exit(EXIT_FAILURE);
    }

    bzero(buffer,256);
    n = read(s,buffer,255);

    if (n < 0) {
		perror("ERROR reading from socket");
		exit(EXIT_FAILURE);
	}

	printf("%s\n",buffer);
    peer_ip = buffer;


	// getaddrinfo(peer_ip, PORT_NO, &info, &server);
	// //open socket connection with peer, send and receive message
	// int new_s = socket(server->ai_family, server->ai_socktype, 0);

	// bzero(buffer,256);
	// strcpy(buffer, "hello");
 //    n = write(new_s,buffer,strlen(buffer));

 //    if (n < 0) {
	// 	perror("ERROR writing to socket");
	// 	exit(EXIT_FAILURE);
 //    }

 //    bzero(buffer,256);
 //    n = read(new_s,buffer,255);

 //    if (n < 0) {
	// 	perror("ERROR reading from socket");
	// 	exit(EXIT_FAILURE);
	// }

	// printf("%s\n",buffer);

	return 0;

}
