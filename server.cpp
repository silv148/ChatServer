#include <iostream>
#include <string>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

int main(int argc, char const* argv[]) {
	int client;
	int server;

	struct sockaddr_in server_address;

	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0) {
		std::cout << "SERVER ERROR: establishing socket error.";
		exit(0);
	}

	std::cout << "SERVER: SOcket for server was successdully created.";
	server_address.sin_port = htons(8080);
}