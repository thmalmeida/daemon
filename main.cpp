#include "ntp.hpp"

#include "socket_udp.hpp"
#include "socket_tcp.hpp"
#include "net_utils.hpp"


#include <iostream>
// #include <thread>
#include <string>

void client_tcp(const char *addr, int port) {
	SOCKET_TCP client(addr, port);

	client.connect_to_server();

	char buffer[40];

	while(1) {
		printf("Enter something: ");
		memset(buffer, 0, sizeof(buffer));
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strcspn(buffer, "\n")+1] = '\0';
		// printf("buffer length: %lu\n", strlen(buffer));
		// std::cout << "fgets: " << buffer << std::endl;
		client.send0(buffer, strlen(buffer));
	}

}
void server_tcp(int port) {

	SOCKET_TCP server(port);

	server.run();

	server.close_all();

	printf("Server closed!\n");
}

void client_udp(const char *addr, int port) {
	SOCKET_UDP client(addr, port);

	char buffer[40];

	while(1) {
		printf("Enter something: ");
		memset(buffer, 0, sizeof(buffer));
		fgets(buffer, sizeof(buffer), stdin);
		buffer[strcspn(buffer, "\n")+1] = '\0';
		// printf("buffer length: %lu\n", strlen(buffer));
		// std::cout << "fgets: " << buffer << std::endl;
		client.send(buffer, strlen(buffer));
		// client.send(buffer, sizeof(buffer));
		// client.receive(buffer, &len);
	}
}
void server_udp(int port) {
	SOCKET_UDP server(port);

	// enter in a infinite while
	server.listen_port();
}

void ntp_test(void) {
	NTP ntp_client;
	// ntp_client.server_name(NTP_SERVER, 123);
	
	ntp_client.server_port(123);
	ntp_client.server_name(NTP_SERVER);
	// ntp_client.server_addr("200.160.7.186");
	ntp_client.timeout(2);
	ntp_client.fetch();

	// printf("Size of NTP class is: %ld\n", sizeof(ntp_client));
}
void endianess_test_show(void) {
	uint32_t y, x = 0x12345678;

	printf("endianess of x 0x%04x is  ", x);
	netutils::endianess_show(x);

	y = htonl(x);
	printf("endianess of y 0x%04x is  ", y);
	netutils::endianess_show(y);
}
void ext_trig(void) {
	SOCKET_UDP client0;

	client0.addr("127.0.0.1", 6802);

	const char msg[40] = "19|on+3|1|Detected_2|text1|text2";

	client0.send(msg, sizeof(msg));
	printf("sent\n");
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

	switch (argc) {
		case 1: {
			ntp_test();
			// ext_trig();
			break;
		}
		case 2: { // Server mode
			// server_udp(atoi(argv[1]));
			// std::cout << "Server mode listen on port: " << "" << std::endl;
			server_tcp(atoi(argv[1]));
			break;
		}
		case 3:
			// Client mode
			std::cout << "Client mode\n" << std::endl;
			// client_udp(argv[1], atoi(argv[2]));
			client_tcp(argv[1], atoi(argv[2]));
			break;

		default:
			std::cerr << "missing parameters" << std::endl;
			break;
	}

	return 0;
}
