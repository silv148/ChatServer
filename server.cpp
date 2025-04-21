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

#define ERROR_S "SERVER ERROR: "
#define DEFAULT_PORT 1605
#define BUFFER_SIZE 1024
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

bool is_client_connection_closed(const char* msg) {
	for(int i = 0; i < strlen(msg); i++)
		if (msg[i] == CLIENT_CLOSE_CONNECTION_SYMBOL)
			return true;

	return false;
}

int main(int argc, char const* argv[]) {
	int client;
	int server;

	struct sockaddr_in server_address;

	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0) {
		std::cout << ERROR_S <<"establishing socket error.";
		exit(0);
	}

	std::cout << "SERVER: Socket for server was successdully created.";
	
	server_address.sin_port = htons(DEFAULT_PORT);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htons(INADDR_ANY);

	int ret = bind(client, reinterpret_cast<struct sockaddr*>(&server_address),
		sizeof(server_address));

	if (ret < 0) {
		std::cout << ERROR_S <<"binding connection. Socket has already been establishing.";
		return -1;
	}
	
	socklen_t size = sizeof(server_address);
	std::cout << "SERVER: Listening clients...";
	listen(client, 1);

	server = accept(client, reinterpret_cast<struct sockaddr*>(&server_address), &size);
	if (server < 0)
		std::cout << ERROR_S <<"Can't accepting connection.";

	char buffer[BUFFER_SIZE];
	bool isExit=false
	while(server > 0) {
		strcpy(buffer, "=? Server connected.\n");
		send(server, buffer, BUFFER_SIZE, 0);
		std::cout << "=> Connected to client. #1\n" << "Enter # to end connection.\n";

		std::cout << "Client: "
		recv(server, buffer, BUFFER_SIZE, 0);
		std::cout << buffer << std::endl;
		if(is_client_connection_closed(buffer))
			isExit=true;

		while(!isExit) {
			std::cout << "Client: ";
			std::cin.getline(buffer, BUFFER_SIZE);
			send(server, buffer, BUFFER_SIZE, 0);
			if(is_client_connection_closed(buffer))
				break;

			std::cout << "Client: ";
			recv(server, buffer, BUFFER_SIZE, 0);
			std::cout << buffer << std::endl;
			if(is_client_connection_closed(buffer))
				break;
		}
		std::cout << std::endl;
		std::cout << "SERVER: Closing connection...\n";
		isExit=false;
		exit(1);
	}
	return 0;
}