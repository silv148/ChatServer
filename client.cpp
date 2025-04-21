#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

#define ERROR_S "CLIENT ERROR: "
#define SERVER_IP "127.0.0.1"
#define DEFAULT_PORT 1601
#define SERVER_CLIENT_CLOSE_CONNECTION_SYMBOL '#'
#define ERROR_S "SERVER ERROR: "
#define BUFFER_SIZE 1024

bool is_client_connection_closed(const char* msg) {
	for(int i = 0; i < strlen(msg); i++)
		if (msg[i] == SERVER_CLIENT_CLOSE_CONNECTION_SYMBOL)
			return true;

	return false;
}

int main(argc, const char* argv[]) {
	int client;

	struct sockaddr_in server_address;
	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0) {
		std::cout << ERROR_S <<"establishing socket error.";
		exit(0);
	}

	server_address.sin_port = htons(8080);
	server_address.sin_family = AF_INET;
	inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);
	server_address.sin_addr.s_addr = htons(INADDR_ANY);

	std::cout << std::endl
		<< "Client socket was successfully created." << std::endl;

	int ret = connect(client, reinterpret_cast<struct sockaddr*>(&server_address),
		sizeof(server_address));
	if(ret == 0)
		std::cout <<"=> Connection to server " 
			<< inet_ntoa(server_address.sin_addr) <<
			<< "with port number " << DEFAULT_PORT << std::endl;
	
	char buffer[BUFFER_SIZE];
	std::cout << "Waiting for server confirmation..." << std::endl;
	recv(client, buffer, BUFFER_SIZE, 0);
	std::cout << "=> Connection established.\n"
		<< "Enter" << SERVER_CLIENT_CLOSE_CONNECTION_SYMBOL
		<< " to close connection." << std::endl;
		
	while (true) {
		std::cout << "Client: ";
		std::cin.getline(buffer, BUFFER_SIZE);
		send(client, buffer, BUFFER_SIZE, 0);
		if (is_client_connection_closed(buffer)) {
			std::cout << "Closing connection..." << std::endl;
			break;
		}

		std::cout << "Server: ";
		recv(client, buffer, BUFFER_SIZE, 0);
		std::cout << buffer << std::endl;
		if (is_client_connection_closed(buffer)) {
			std::cout << "Closing connection..." << std::endl;
			break;
		}
		std::cout << std::endl;
	}
	close(client);
	std::cout << "Connection closed." << std::endl;
	return 0;
}