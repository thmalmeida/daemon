#include "socket_udp.hpp"
#include "socket_tcp.hpp"

#include "ntp.hpp"

#include <iostream>
// #include <string>

void client_tcp(void) {
	SOCKET_TCP client("127.0.0.1", 5003);

	client.connect_to_server();

}
void client_udp(const char *addr, int port) {
	SOCKET_UDP client(addr, port);

	char buffer[40];
	int len = 0;

	int count = 8;
	// while (1) {
		sprintf(buffer, "%d", count++);
		client.send(buffer, sizeof(buffer));
		// client.receive(buffer, &len);

		// sleep(1);
	// }
}

void server_tcp(void) {
	SOCKET_TCP server(5003);

	while(1) {
		server.listen_port();

		server.wait_connection();
	}

	server.close_port();

	printf("Server closed!\n");
}
void server_udp(int port) {
	SOCKET_UDP server(port);

	// enter in a infinite while
	server.listen_port();
}

void client_ntp(void) {

}


// DNS resolvers
void resolv1(void) {
	hostent* myHostent = gethostbyname("google.com");
	if (!myHostent) {
		std::cout << "gethostbyname() failed" << "\n";
	} else {
		std::cout << myHostent->h_name << "\n";
		char ip[INET6_ADDRSTRLEN];

		for (unsigned int i = 0; myHostent->h_addr_list[i] != NULL; ++i) {
			std::cout << inet_ntop(myHostent->h_addrtype, myHostent->h_addr_list[i], ip, sizeof(ip)) << "\n";
		}
	}
}
void* getSinAddr(addrinfo *addr)
{
	switch (addr->ai_family)
	{
		case AF_INET:
		return &(reinterpret_cast<sockaddr_in*>(addr->ai_addr)->sin_addr);

		case AF_INET6:
		return &(reinterpret_cast<sockaddr_in6*>(addr->ai_addr)->sin6_addr);
	}

	return NULL;
}
void resolv2(const char *addr) {
	addrinfo hints = {};
	hints.ai_flags = AI_CANONNAME;		// input flags
	hints.ai_family = AF_UNSPEC;		// protocol family for socket
	hints.ai_socktype = SOCK_STREAM;	// socket type
	hints.ai_protocol = IPPROTO_TCP;	// protocol for socket

	addrinfo *res, *addr_;

	int ret = getaddrinfo(addr, NULL, &hints, &res);
	if (ret != 0) {
		std::cout << "getaddrinfo() failed: " << gai_strerror(ret) << "\n";
	} else {
		std::cout << res->ai_canonname << "\n";

		addr_ = res;
		char ip[INET6_ADDRSTRLEN];

		do {
			std::cout << inet_ntop(addr_->ai_family, getSinAddr(addr_), ip, sizeof(ip)) << std::endl;
			addr_ = addr_->ai_next;
		} while (addr_);

		freeaddrinfo(res);
	}	
}

// parameters check and select
int parameters_select(int argc, char *argv[]) {
	std::cout << "argc: " << argc << std::endl;

	for(int i = 0; i< argc; i++) {
		std::cout << argv[i] << std::endl;
	}

	if(argc < 2) {
		std::cerr << "missing input arguments" << std::endl;
		return EXIT_FAILURE;
	}

	std::string str1;

	char str_cmd[2];

	strcpy(str_cmd, argv[1]);

	return 0;
}

int main(int argc, char *argv[]) {

	// if(parameters_select(argc, argv)) {
	// 	return 1;
	// }

	// char str[] = "thmalmeida.us.to";
	// resolv2(str);
	// client_udp();
	// client_tcp();

	// Server mode
	if(argc > 1) {
		int port = atoi(argv[1]);

		// Server side
		server_udp(port);
	} else {
	// Client mode
		client_udp("127.0.0.1", 9092);
	}


	return 0;
}
