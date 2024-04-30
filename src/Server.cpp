/**======================
*            42sp
* @file      : Server.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 17/04/2024
*========================**/

#include "../include/includes.hpp"
#include "../include/error.hpp"
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

void Server::setupServer() {
	this->setupSocket();

	while (_signal) {
		
		if(poll(&_fds[0], _fds.size(), -1) == -1 && _signal)
			throw std::runtime_error(ERRMSG_POLL);

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
	closeFds();
}

void Server::listenClient(int clientFD) {
	char buff[513];
	bzero(buff, 513);

	ssize_t byte = recv(clientFD, buff, 513, 0);
	if(byte <= 0){
		Server::clearClients(clientFD);
		close(clientFD);
		return;
	}

	buff[byte] = '\0';
	Server::selectOptions(buff, clientFD);
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
		throw std::runtime_error(ERRMSG_SOCKET);

	if(setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opval, sizeof(int)) < 0)
		throw std::runtime_error(ERRMSG_SETSOCKOPT);

	if(fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(ERRMSG_FCNTL);

	if (bind(_sockfd, (struct sockaddr *) &serv, sizeof(serv)) < 0)
		throw std::runtime_error(ERRMSG_BIND);

	if(listen(_sockfd, SOMAXCONN) < 0)
		throw std::runtime_error(ERRMSG_LISTEN);

	newPoll.fd = _sockfd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;

	_fds.push_back(newPoll);
}

void Server::acceptNewClient() {
	Client client;
	struct sockaddr_in cliAdd;
	struct pollfd newPoll;
	socklen_t clilen = sizeof(cliAdd);

	int newsockfd = accept(_sockfd, (struct sockaddr *) &cliAdd, &clilen);
	
	if (newsockfd < 0)
		throw std::runtime_error(ERRMSG_ACCEPT);

	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(ERRMSG_FCNTL);

	newPoll.fd = newsockfd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	
	client.setClientFD(newsockfd);
	client.setClientIP(inet_ntoa(cliAdd.sin_addr));
	_clients.push_back(client);
	_fds.push_back(newPoll);
}

void Server::clearClients(int clientFd) {
	for(size_t i = 0; i < _fds.size(); i++){
		if (_fds[i].fd == clientFd) {
			_fds.erase(_fds.begin() + i); 
			break;
		}
 }
	for(size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getClientFD() == clientFd) {
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
}

void Server::closeFds() {
	for(size_t i = 0; i < _clients.size(); i++) {
			close(_clients[i].getClientFD());
	}
	if(_sockfd != -1)
		close(_sockfd);
}

bool Server::_signal = true;
void Server::handleSig(int signum) {
	(void)signum;
	Server::_signal = false;
}

Client& Server::getClient(int clientFd) {
	for(size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].getClientFD() == clientFd) {
			return _clients[i];
		}
	}
	throw std::runtime_error(ERRMSG_CLIENT);
}

// std::string response = "CAP * LS :\r\n";
// send(newsockfd, response.c_str(), response.size(), 0);
// recv(newsockfd, buff, 100000, 0);
// std::cout << buff;