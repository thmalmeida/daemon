#ifndef SOCKET_UDP_HPP__
#define SOCKET_UDP_HPP__

/* C++14 */
// Header of UDP client-server model 
// #include <bits/stdc++.h>

#ifdef ESP32_UC
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#else
// Unix includes
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <unistd.h> // read(), write(), close()

// system
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

// time
#include <time.h>

#endif


#define DESTINATION_HOST "127.0.0.1"
// #define PORT 5000
#define MAXLINE 1024


enum class socket_udp_mode {
	server = 0,
	client
};

void endianess_show(void) {
	uint32_t x = 0x12345678;
	const uint8_t* p = reinterpret_cast<const uint8_t*>(&x);
	printf("%02X%02X%02X%02X\n", p[0], p[1], p[2], p[3]);
}


class SOCKET_UDP {
public:

	// server mode
	SOCKET_UDP(uint16_t port) : port_(port) {
		mode_ = socket_udp_mode::server;
		init();
		bind_server();
	}

	// client mode
	SOCKET_UDP(const char* server_addr, uint16_t port) : port_(port)  {
		mode_ = socket_udp_mode::client;
		init();
	}

	~SOCKET_UDP(void) {

	}

	int init(void) {
	
		// Dummy protocol for TCP (= 0)
		ip_protocol_ = IPPROTO_IP;

		// Creating socket file descriptor and verification
		if((sockfd_ = socket(AF_INET, SOCK_DGRAM, ip_protocol_)) < 0 ) {
			printf("socket creation failed"); 
			return 1; 
		} else {
			printf("socket created!\n");
		}

		// Set timeout
		struct timeval timeout;
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;
		setsockopt (sockfd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

		// clear server and client addr structure
		memset(&server_addr_, 0, sizeof(server_addr_)); 
		memset(&client_addr_, 0, sizeof(client_addr_));


		// Filling server information 
		server_addr_.sin_family = AF_INET;				// set IPv4 
		server_addr_.sin_port = htons(port_);			// set port to listen
		if(mode_ == socket_udp_mode::server) {			// Server mode
			server_addr_.sin_addr.s_addr = INADDR_ANY;	//in_addr_t or uint32_t		
			printf("Server mode\n");
		} else if (mode_ == socket_udp_mode::client) {	// Client mode
			// server_addr_.sin_addr.s_addr = inet_addr(DESTINATION_HOST);

			// IPv4 string of inet_ntop() and inet_pton()
			char server_addr_str_[INET_ADDRSTRLEN];

			// store this IP address in sa:
			inet_pton(AF_INET, DESTINATION_HOST, &(server_addr_.sin_addr));

			// now get it back and print it
			inet_ntop(AF_INET, &(server_addr_.sin_addr), server_addr_str_, INET_ADDRSTRLEN);
			// char *inet_ntoa(struct in_addr in);
			// int inet_aton(const char *cp, struct in_addr *inp);
			// in_addr_t inet_addr(const char *cp);
			printf("Client mode destination addr: %s\n", server_addr_str_);


			// if(setsockopt(sockfd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
			// 	printf("setcokopt error");
			// }
		}

		return 0;
	}

	int bind_server(void) {
		// Bind the socket with the server address 
		if (bind(sockfd_, (struct sockaddr *)&server_addr_, sizeof(server_addr_)) < 0 ) {
				printf("bind failed\n");
				return 1;
		} else {
			printf("Bind socket with server successfully!\n");
			return 0;
		}
	}
	int listen_port(void) {
		printf("listening on port: %d\n", port_);
		while(1) {
			len_ = sizeof(client_addr_);  //len is value/result 

			n_bytes_ = recvfrom(sockfd_, (char *)buffer_, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &client_addr_, &len_);
			buffer_[n_bytes_] = '\0';

			if(n_bytes_) {
				printf("%s:%d << %s\n",inet_ntoa(client_addr_.sin_addr), ntohs(client_addr_.sin_port), buffer_);
			}

			// send back
			send_back("hey2!", 5);
			// fflush(stdout);
		}

		return 0;
	}

	int send(const char *str, int size) {
		// sendto(sockfd_, (const char *)hello_1, strlen(hello_1), MSG_CONFIRM, (const struct sockaddr *) &server_addr_, sizeof(server_addr_)); 

		if(sendto(sockfd_, str, size, MSG_CONFIRM, (const struct sockaddr *) &server_addr_, sizeof(server_addr_))) {
			printf("sent: %s\n", str);
		}

		// send back
		// sendto(sockfd_, (const char *)hello_2, strlen(hello_2), MSG_CONFIRM, (const struct sockaddr *) &client_addr_, len_); 
		// printf("Hello message sent.\n");
	
		return 0;
	}
	int send_back(const char *str, int size) {
		// sendto(sockfd_, (const char *)hello_1, strlen(hello_1), MSG_CONFIRM, (const struct sockaddr *) &server_addr_, sizeof(server_addr_)); 
		sendto(sockfd_, str, size, MSG_CONFIRM, (const struct sockaddr *) &client_addr_, sizeof(client_addr_)); 
		printf("%s:[%d] >> %s\n",inet_ntoa(client_addr_.sin_addr), ntohs(client_addr_.sin_port), str);

		// send back
		// sendto(sockfd_, (const char *)hello_2, strlen(hello_2), MSG_CONFIRM, (const struct sockaddr *) &client_addr_, len_); 
		// printf("Hello message sent.\n");
	
		return 0;
	}
	int receive(char *str_rx, int *size) {
		n_bytes_ = recvfrom(sockfd_, (char *)str_rx, MAXLINE, MSG_WAITALL, (struct sockaddr *) &server_addr_, &len_); 
		buffer_[n_bytes_] = '\0';
		*size = n_bytes_;
		printf("Receive: %s\n", str_rx);

		return 0;
	}

	void close_port(void) {
		close(sockfd_);
	}

private:

	// server parameters
	int sockfd_;									// socket descriptor
	struct sockaddr_in server_addr_, client_addr_;	// server and client struct addr
	socklen_t len_;									// packet size
	int n_bytes_;									// answer for reception

	uint16_t port_;									// destination port/server bind port
	int ip_protocol_ = 0;							// Standard well-defined IP protocols on in.h
	char buffer_[MAXLINE];

	socket_udp_mode mode_;							// behavior client/server
};

#endif