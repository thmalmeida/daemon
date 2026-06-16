#ifndef SOCKET_TCP_HPP__
#define SOCKET_TCP_HPP__

/* C++14 */
// Header of TCP client-server model 
// #include <bits/stdc++.h>	// not used 

// #define ESP32_UC

#ifdef ESP32_UC
// #include <sys/param.h>

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"

// #include "esp_system.h"
// #include "esp_event.h"
// #include "esp_log.h"

// #include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#else
// Unix includes
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <arpa/inet.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h> // read(), write(), close()

#include <sys/wait.h>
#include <signal.h>
#endif

/* Refs.:
*	https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
*/

/*	Socket TCP sequence
	server: socket ----	setsockopt ----	bind --	listen ----	accept ----	send/recv
									   	  		   |
	client: socket ----------------------------	connect	---------------	send/recv
*/

#define SOCKET_TCP_DEBUG

#define LOCALHOST			"127.0.0.1"
#define BACKLOG				10
#define MAX 				80 
#define MAXLINE_TCP			1024

#define SA struct sockaddr

enum class socket_tcp_mode {
	server = 0,
	client
};

void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	else 
		return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

class SOCKET_TCP {
public:

	// server mode
	SOCKET_TCP(uint16_t port) : port_(port) {
		mode_ = socket_tcp_mode::server;
		init();
		bind_server();
		listen_port();
	}

	// client mode
	SOCKET_TCP(const char* server_addr, uint16_t port) : port_(port)  {
		mode_ = socket_tcp_mode::client;
		init();
	}

	~SOCKET_TCP(void) {
	}

	// Create a socket only
	int init(void) {
		// Dummy protocol for TCP (= 0)
		ip_protocol_ = IPPROTO_IP;

		// clear server and client addr structure
		// bzero( (char*) &remote_addr_, sizeof(remote_addr_));
		memset(&remote_addr_, 0, sizeof(remote_addr_));
		memset(&local_addr_, 0, sizeof(local_addr_));

		// memset(&hints_, 0, sizeof(hints_));
		// hints_.ai_family = AF_UNSPEC;
		// hints_.ai_socktype = SOCK_STREAM;
		// hints_.ai_flags = AI_PASSIVE;		// use my IP;

		// if((int rv = getaddrinfo(NULL, PORT, &hints_, &servinfo_)) != 0) {
		// 	printf("getaddrinfo error\n");
		// }

		// for(p_ = servinfo_; p != NULL; p = p->ai_next) {
		// 	if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
		// 		perror("server: socket");
		// 		continue;
		// 	}

		// 	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		// 		perror("setsockopt");
		// 		exit(1);
		// 	}

		// 	if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
		// 		close(sockfd);
		// 		perror("server: bind");
		// 		continue;
		// 	}
		// 	break;
		// }
		// freeaddrinfo(servinfo_);

		// Creating socket file descriptor and verification
		if((sockfd_ = socket(AF_INET, SOCK_STREAM, ip_protocol_)) < 0) {
			#ifdef SOCKET_TCP_DEBUG
			printf("socket creation failed");
			// std::cerr << "socket creation failed" << std::endl;
			#endif
			return 1; 
		} else {
			// std::clog << "socket created!" << std::endl;
			printf("TCP socket created!\n");
			return 0;
		}
	}
	void port(uint16_t port) {
		if(mode_ == socket_tcp_mode::server) {			// Server mode
			local_addr_.sin_port = htons(port_);		// set port to listen
		} else if (mode_ == socket_tcp_mode::client) {	// Client mode
			remote_addr_.sin_port = htons(port_);		// set port to listen
		}
	}
	int addr(const char *addr_str) {
		if(mode_ == socket_tcp_mode::server) {			// Server mode
			local_addr_.sin_addr.s_addr = htonl(INADDR_ANY);	//in_addr_t or uint32_t		
			// local_addr_.sin_family = AF_UNSPEC;			// use IPv4 or IPv6, whichever
			local_addr_.sin_family = AF_INET;			// IPv4
			// local_addr_.ai_socktype = SOCK_STREAM;		// Uses TCP socket stream
			// local_addr_.ai_flags = AI_PASSIVE;			// fill in my IP for me
		} else if (mode_ == socket_tcp_mode::client) {	// Client mode
			// Filling remote/server information 
			remote_addr_.sin_family = AF_INET;			// set IPv4

			// Set ip address - way 1 (deprecated)
			// remote_addr_.sin_addr.s_addr = inet_addr(addr_str);

			// Set ip address - way 2
			inet_pton(remote_addr_.sin_family, addr_str, &(remote_addr_.sin_addr));

			// --- The next following lines is for debug purpose only. Get it back and print it ---
			// IPv4 string of inet_ntop() and inet_pton()
			// char remote_addr_str[INET_ADDRSTRLEN];
			
			// inet_ntop(remote_addr_.sin_family, &(remote_addr_.sin_addr), remote_addr_str, sizeof(remote_addr_str));
			// char *inet_ntoa(struct in_addr in);

			// printf("Client mode. Remote addr: %s:%d\n", remote_addr_str_, port_);

			/*
			use 
			inet_ntop() — Convert Internet address format from binary to text
			or
			inet_pton()
			*/


			/* Refs.: https://www.gta.ufrj.br/ensino/eel878/sockets/inet_ntoaman.html (DEPRECATED)
			struct sockaddr_in antelope;
			char *some_addr;

			inet_aton("10.0.0.1", &antelope.sin_addr); // store IP in antelope

			some_addr = inet_ntoa(antelope.sin_addr); // return the IP
			printf("%s\n", some_addr); // prints "10.0.0.1"

			// and this call is the same as the inet_aton() call, above:
			antelope.sin_addr.s_addr = inet_addr("10.0.0.1");
			*/
		}

		return 0;
	}
	int addr(const char *addr_str, uint16_t _port) {
		port(_port);

		return addr(addr_str);
	}
	
	int timeout(int t_sec) {
		// Set timeout
		struct timeval timeout;
		timeout.tv_sec = t_sec;
		timeout.tv_usec = 0;
		if(setsockopt(sockfd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
			return 1;	// return fail;
		} else {
			return 0;	// return success;
		}	
	}

	int bind_server(void) {
		// Bind the socket with the server address 
		if (bind(sockfd_, (struct sockaddr *)&local_addr_, sizeof(local_addr_)) < 0 ) {
			#ifdef SOCKET_TCP_DEBUG
			printf("bind failed\n");
			#endif
			return 1;
		} else {
			#ifdef SOCKET_TCP_DEBUG
			printf("Bind socket with server successfully!\n");
			#endif
			return 0;
		}
	}
	int listen_port(void) {
		if(listen(sockfd_, BACKLOG) != 0) {
			printf("Listen failed...\n");
			return 1;
		} else {
			printf("listening on port: %d\n", port_);
			return 0;
		}
	}

	int send0(const void *b_tx, size_t len) {
		send(sockfd_, b_tx, len, 0);

		return 0;
	}
	int receive(void *b_tx, size_t* size) {
		size_t _len;
		recv(sockfd_, b_tx, _len, 0);

		*size = _len;

		return 0;
	}
	// TCP server - wait accept
	int wait_connection(void) {
		len_ = sizeof(local_addr_);  //len is value/result

		sin_size_ = sizeof(their_addr_);

		// Accept the data packet from client and verification
		printf("TCP Server: waiting connection...\n");
		connfd_ = accept(sockfd_, (struct sockaddr*)&their_addr_, &sin_size_);

		if(connfd_ == -1) {
			printf("TCP server accept failed...\n");
			return 1;
		} else {
			printf("TCP server accept connection!\n");
			return 0;
		}

		char s[INET6_ADDRSTRLEN];
		inet_ntop(their_addr_.ss_family, get_in_addr((struct sockaddr *)&their_addr_), s, sizeof(s));
		printf("server: got connection from %s\n", s);
	}
	// TCP client - connect to a server
	int connect_to_server(void) {
		// connect the client socket to server socket
		if (connect(sockfd_, (SA*)&remote_addr_, sizeof(remote_addr_)) != 0) {
			printf("connection with the server failed...\n");
			return 1;
		} else {
			printf("connected to the server..\n");
			return 0;
		}
	}

	void close_port(void) {
		close(sockfd_);
	}

	// Chat function for TCP protocol
	void f1(void) {
		char buffer[MAX];
		int n;

		while(1) {
			bzero(buffer, MAX); 

			printf("wait receive msg\n");
			// read the message from client and copy it in buffer 
			read(connfd_, buffer, sizeof(buffer));

			printf("%s:%d  %s\n",inet_ntoa(local_addr_.sin_addr), ntohs(local_addr_.sin_port), buffer_);

			

		}
	}

private:

	// server parameters
	int sockfd_;									// socket descriptor, listen on sockfd and new connection on new_fd_
	int connfd_;									// connection descriptor for TCP

	struct addrinfo hints_, *servinfo_, *p_;
	struct sockaddr_storage their_addr_;				// connector's address information
	socklen_t sin_size_;
	struct sigaction sa_;
	struct sockaddr_in remote_addr_, local_addr_;	// server and client struct addr

	socklen_t len_;									// packet size
	int n_bytes_;									// answer for reception

	uint16_t port_;									// destination port/server bind port
	int ip_protocol_;								// Standard well-defined IP protocols on in.h
	char buffer_[MAXLINE_TCP];

	int yes = 1;
	char addr_str_[INET6_ADDRSTRLEN];

	socket_tcp_mode mode_;							// behavior client/server
};

#endif