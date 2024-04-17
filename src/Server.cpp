/**======================
*            42sp
* @file      : Server.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 17/04/2024
*========================**/

#include "../include/Server.hpp"

void Server::setPort(char *input){
	std::stringstream iss;
	iss << input;
	int port;
	iss >> port;
	if(port < 0 || port > 65535){
		std::cerr << "Invalid port" << std::endl;
		exit(1);
	}
	_port = port;
}

int Server::getPort(){
	return _port;
}

void Server::setPassword(char *input){
	_password = input;
}

std::string Server::getPassword(){
	return _password;
}

void Server::socketInit(){
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0){
		perror("socket");
		exit(1);
	}
	_serv_addr.sin_family = AF_INET;
	_serv_addr.sin_addr.s_addr = INADDR_ANY;
	_serv_addr.sin_port = htons(_port);
	if (bind(_sockfd, (struct sockaddr *) &_serv_addr, sizeof(_serv_addr)) < 0){
		perror("bind");
		exit(1);
	}
	listen(_sockfd, std::numeric_limits<int>::max());
}

void Server::receive(){
	while (true){
		int newsockfd;
		newsockfd = accept(_sockfd, NULL, NULL);
		if (newsockfd < 0){
			perror("accept");
			std::exit(1);
		}
		char buff[100000];
		recv(newsockfd, buff, 100000, 0);
		std::cout << buff;
		std::string response = "CAP * LS :\r\n";
		send(newsockfd, response.c_str(), response.size(), 0);
		recv(newsockfd, buff, 100000, 0);
		std::cout << buff;
	}
}

