/**======================
*            42sp
* @file      : Server.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 17/04/2024
*========================**/

#include "../include/Server.hpp"

void Server::setPort(char *input) {
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

int Server::getPort() {
	return _port;
}

void Server::setPassword(char *input) {
	_password = input;
}

std::string Server::getPassword() {
	return _password;
}

void Server::setupServer(char **input) {
	this->setPort(input[1]);
	this->setPassword(input[2]);
	_serv_addr.sin_family = AF_INET;
	_serv_addr.sin_addr.s_addr = INADDR_ANY;
	_serv_addr.sin_port = htons(_port);
}

void Server::setupSocket() {
	int opval = 1;
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "Socket: " << _sockfd << std::endl;
	if (_sockfd < 0)
		throw std::runtime_error("Error opening socket");
	if(setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opval, sizeof(int)) < 0)
		throw std::runtime_error("Error on setsockopt");
	if (bind(_sockfd, (struct sockaddr *) &_serv_addr, sizeof(_serv_addr)) < 0)
		throw std::runtime_error("Error on binding");
	listen(_sockfd, std::numeric_limits<int>::max());
}

void Server::receive() {
	int newsockfd;
	char buff[100000];
	newsockfd = accept(_sockfd, NULL, NULL);
	while (true){
		if (newsockfd < 0){
			perror("accept");
			throw std::runtime_error("");
		}
		if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
			throw std::runtime_error("Error on fcntl");
		recv(newsockfd, buff, sizeof(buff), 0);
		std::cout << buff;
		this->parseOptions(std::string(buff));
		bzero(buff, sizeof(buff));
	}
}

void Server::parseOptions(std::string buff) {

	void (Server::*fct_ptr[10])(std::string buff) = {
		&Server::user, &Server::nick, &Server::join, &Server::part, &Server::names, &Server::list, &Server::privmsg, &Server::quit, &Server::oper, &Server::mode
	};

	std::string option = buff.substr(0, buff.find(" "));
	std::string requests[] = {"USER", "NICK", "JOIN", "PART", "NAMES", "LIST", "PRIVMSG", "QUIT", "OPER", "MODE"};
	int i;
	for(i = 0; i < 10 && requests[i] != option; i++);
	if(i == 10){
		std::cerr << "Invalid request" << std::endl;
		i = 0;
		return;
	}
	(this->*fct_ptr[i])(buff.substr(buff.find(" ") + 1));
	i = 0;
}


void Server::join(std::string channel) {
	std::cout << "Joining channel: " << channel << std::endl;
}

void Server::part(std::string channel) {
	std::cout << "Parting channel: " << channel << std::endl;
}

void Server::names(std::string channel) {
	std::cout << "Listing names in channel: " << channel << std::endl;
}

void Server::list(std::string channel) {
	(void)channel;
	std::cout << "Listing all channels" << std::endl;
}

void Server::privmsg(std::string channel) {
	std::cout << "Sending message to channel: " << channel << " - " << std::endl;
}

void Server::quit(std::string channel) {
	(void )channel;
	std::cout << "Quitting" << std::endl;
}

void Server::nick(std::string nickname) {
	std::cout << "Changing nickname to: " << nickname << std::endl;
}

void Server::user(std::string username) {
	std::cout << "Setting username to: " << username << std::endl;
}

void Server::oper(std::string user) {
	std::cout << "Opering user: " << user << std::endl;
}

void Server::mode(std::string channel) {
	std::cout << "Setting mode: " << " in channel: " << channel << std::endl;
}


// std::string response = "CAP * LS :\r\n";
// send(newsockfd, response.c_str(), response.size(), 0);
// recv(newsockfd, buff, 100000, 0);
// std::cout << buff;