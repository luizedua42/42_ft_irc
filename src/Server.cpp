/**======================
*            42sp
* @file      : Server.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 17/04/2024
*========================**/

#include "../include/includes.hpp"

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

void Server::setupServer() {
	this->setupSocket();

	while (true) {
		
		if(poll(&_fds[0], _fds.size(), -1) < 0)
			throw std::runtime_error("Error on poll");

		for(size_t i = 0 ; i < _fds.size(); i++) {
			if(_fds[i].revents & POLLIN) {
				if(_fds[i].fd == _sockfd)
					acceptNewClient();
				else {
					listenClient(_fds[i].fd);
				}
			}
		}
	}
	//close all fds
}

void Server::listenClient(int clientFD) {
	char buff[513];
	bzero(buff, 513);

	ssize_t byte = recv(clientFD, buff, 513, 0);
	if(byte <= 0){
		//clear client
		close(clientFD);
		return;
	}

	buff[byte] = '\0';
	selectOptions(buff);
}

void Server::setupSocket() {
	struct sockaddr_in serv;
	struct pollfd newPoll;
	int opval = 1;

	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(_port);

	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "Socket: " << _sockfd << std::endl;
	if (_sockfd < 0)
		throw std::runtime_error("Error opening socket");

	if(setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opval, sizeof(int)) < 0)
		throw std::runtime_error("Error on setsockopt");
	if(fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error on fcntl");
	if (bind(_sockfd, (struct sockaddr *) &serv, sizeof(serv)) < 0)
		throw std::runtime_error("Error on binding");
	if(listen(_sockfd, SOMAXCONN) < 0)
		throw std::runtime_error("Error on listen");
	newPoll.fd = _sockfd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;

	_fds.push_back(newPoll);
}

void Server::acceptNewClient() {
	Client client;
	struct sockaddr_in cliAdd;
	struct pollfd newPoll;

	int newsockfd = accept(_sockfd, NULL, NULL);
	if (newsockfd < 0){
		perror("accept");
		throw std::runtime_error("");
	}
	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error on fcntl");

	newPoll.fd = newsockfd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	
	client.setClientFD(newsockfd);
	client.setClientIP(inet_ntoa(cliAdd.sin_addr));
	_fds.push_back(newPoll);
}

std::vector<std::string> Server::parseOptions(std::string str) {
	std::string word;
	std::stringstream ss(str);
	std::vector<std::string> splitted;
	while (std::getline(ss, word, ' '))
		if(word.find('\r') != std::string::npos)
			splitted.push_back(word.substr(0, word.find('\r')));
		else
			splitted.push_back(word);

	return splitted;
}

void Server::selectOptions(std::string buff) {
	int i;

	void (Server::*fct_ptr[10])(std::vector<std::string>) = { 
		&Server::user, &Server::nick, &Server::join, &Server::privmsg, &Server::quit,
		&Server::oper, &Server::mode, &Server::topic, &Server::invite, &Server::kick
	};

	std::string option = buff.substr(0, buff.find(" "));
	std::string requests[] = {"USER", "NICK", "JOIN", "PRIVMSG", "QUIT", "OPER", "MODE", "TOPIC", "INVITE", "KICK"};
	
	for(i = 0; i < 10 && requests[i] != option; i++);
	
	if(i == 10){
		std::cerr << "Invalid request: " << option << std::endl;
		i = 0;
		return;
	}

	(this->*fct_ptr[i])(parseOptions(buff.substr(buff.find(" ") + 1)));
	i = 0;
}

void Server::join(std::vector<std::string> options) {
	std::string channel = options[0];

	std::cout << "Joining channel: " << channel << std::endl;
}

void Server::privmsg(std::vector<std::string> options) {
	std::cout << "Sending message to channel: " << options[0] << " - " << std::endl;
}

void Server::quit(std::vector<std::string> options) {
	std::string channel= options[0];

	std::cout << "Quitting" << channel << std::endl;
	close(_sockfd);
	exit(0);
}
//nick
void Server::nick(std::vector<std::string> option) {
	std::string nickname = option[0].substr(0, option[0].find('\r'));

	std::cout << "Changing nickname to: " << nickname << std::endl;
}

void Server::user(std::vector<std::string> option) {
	std::string username = option[0];

	std::cout << "Setting username to: " << username << std::endl;
}

void Server::oper(std::vector<std::string> option) {
	std::cout << "Opering user: " << option[0] << std::endl;
}

void Server::mode(std::vector<std::string> option) {
	std::string channel = option[0];
	std::string mode = option[1];

	std::cout << "Setting mode: " << mode << " in channel: " << channel << std::endl;
}

void Server::topic(std::vector<std::string> option) {
	std::cout << "Setting topic in channel: " << option[0] << std::endl;
}

void Server::invite(std::vector<std::string> option) {
	std::cout << "Inviting user to channel: " << option[0] << std::endl;
}

void Server::kick(std::vector<std::string> option) {
	std::string channel = option[0].substr(0, option[0].find('\r'));
	std::string user = option[1];
	std::cout << "Kicking " << user << " from channel: " << channel << std::endl;
}
// std::string response = "CAP * LS :\r\n";
// send(newsockfd, response.c_str(), response.size(), 0);
// recv(newsockfd, buff, 100000, 0);
// std::cout << buff;