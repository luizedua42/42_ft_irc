/**======================
*            42sp
* @file      : main.c
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 16/04/2024
*========================**/

#include "../include/includes.hpp"
#include <sstream>

int socketMaker(int port){
	int sockfd;
	struct sockaddr_in serv_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		std::cerr << "Error opening socket" << std::endl;
		exit(1);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		// std::cerr << "Error on binding" << std::endl;
		perror("bind");
		exit(1);
	}
	listen(sockfd, std::numeric_limits<int>::max());
	return sockfd;
}

void receiver(int sockfd){
	int newsockfd;
	newsockfd = accept(sockfd, NULL, NULL);
	if (newsockfd < 0){
		std::cerr << "Error on accept" << std::endl;
		std::exit(1);
	}
	char buff[100000];
	recv(newsockfd, buff, 100000, 0);
	std::cout << buff;
	std::string response = "CAP * LS :\r\n";
	send(newsockfd, response.c_str(), response.size(), 0);
	recv(newsockfd, buff, 100000, 0);
	std::cout << buff;

	// close(newsockfd);
}

int main(int ac, char **av){
	std::stringstream iss;
	std::string password;
	if (ac != 3){
		std::cout << "Usage: " << av[0] << " <port> <password>"  << std::endl;
		return 1;
	}
	iss << av[1];
	int port;
	iss >> port;
	if(port > 65535 || port < 1){
		std::cout << "Invalid port number" << std::endl;
		return 1;
	}
	std::cout << "Port: " << port << std::endl;
	password = av[2];
	std::cout << "Password: " << password << std::endl;
	std::cout << "Server is running..." << std::endl;
	int sockfd = socketMaker(port);
	receiver(sockfd);
	// close(sockfd);
	std::cout << "Server is closing..." << std::endl;
	return 0;
}