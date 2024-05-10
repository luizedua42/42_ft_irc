/**======================
*            42sp
* @file      : Server.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 17/04/2024
*========================**/

#include "../headers/mainHeader.hpp"
#include "../headers/Server.hpp"

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
			throw std::runtime_error(ERR_POLL);

		for(size_t i = 0 ; i < _fds.size(); i++) {
			if(_fds[i].revents & POLLIN) {
				if(_fds[i].fd == _sockfd)
					acceptNewUser("User1");
				else {
					listenUser(_fds[i].fd);
				}
			}
		}
	}
	closeFds();
}

void Server::listenUser(int userFD) {
	User* user = getUser(userFD);
	char buff[513];
	bzero(buff, 513);

	ssize_t byte = recv(userFD, buff, 513, 0);
	if(byte < 0){
		user->clientBuff.clear();
		Server::clearUsers(userFD);
		close(userFD);
		return;
	}
	user->clientBuff.clear();
	if(user->clientBuff.empty()) {
		user->clientBuff = buff;
	}
	else
		user->clientBuff.append(buff);
	if(user->clientBuff.find("\n") == std::string::npos) {
		return;
	}
	Server::selectOptions(user->clientBuff, userFD);
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
		throw std::runtime_error(ERR_SOCKET);

	if(setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opval, sizeof(int)) < 0)
		throw std::runtime_error(ERR_SETSOCKOPT);

	if(fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(ERR_FCNTL);

	if (bind(_sockfd, (struct sockaddr *) &serv, sizeof(serv)) < 0)
		throw std::runtime_error(ERR_BIND);

	if(listen(_sockfd, SOMAXCONN) < 0)
		throw std::runtime_error(ERR_LISTEN);

	newPoll.fd = _sockfd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;

	_fds.push_back(newPoll);
}

void Server::acceptNewUser(const char* nickName) {
	User User(nickName);
	struct sockaddr_in cliAdd;
	struct pollfd newPoll;
	socklen_t clilen = sizeof(cliAdd);

	int newsockfd = accept(_sockfd, (struct sockaddr *) &cliAdd, &clilen);
	
	if (newsockfd < 0)
		throw std::runtime_error(ERR_ACCEPT);

	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error(ERR_FCNTL);

	newPoll.fd = newsockfd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	
	User.setuserFD(newsockfd);
	User.setuserIP(inet_ntoa(cliAdd.sin_addr));
	_Users.push_back(User);
	_fds.push_back(newPoll);
}

void Server::clearUsers(int userFD) {
	for(size_t i = 0; i < _fds.size(); i++){
		if (_fds[i].fd == userFD) {
			_fds.erase(_fds.begin() + i); 
			break;
		}
 }
	for(size_t i = 0; i < _Users.size(); i++) {
		if (_Users[i].getuserFD() == userFD) {
			_Users.erase(_Users.begin() + i);
			break;
		}
	}
}

void Server::closeFds() {
	for(size_t i = 0; i < _Users.size(); i++) {
			close(_Users[i].getuserFD());
	}
	if(_sockfd != -1)
		close(_sockfd);
}

bool Server::_signal = true;
void Server::handleSig(int signum) {
	(void)signum;
	Server::_signal = false;
}

User* Server::getUser(int userFD) {
	for(size_t i = 0; i < _Users.size(); i++) {
		if (_Users[i].getuserFD() == userFD) {
			return &_Users[i];
		}
	}
	return NULL;
	//send with(ERR_USER);
}

bool Server::channelExists(const std::string& channelName) const {
    for (std::vector<Channel>::const_iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->getName() == channelName) {
            return true;
        }
    }
    return false;
}

Channel* Server::getChannel(const std::string& channelName) {
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        if (it->getName() == channelName) {
            return &(*it);
        }
    }
	return NULL;
}

void Server::createChannel(const std::string channelName) {
	Channel newChannel(channelName);
	_channels.push_back(newChannel);
}

User* Server::searchUser(std::string nickName) {
	for(size_t i = 0; i < _Users.size(); i++) {
		if (_Users[i].getNickName() == nickName) {
			return &_Users[i];
		}
	}
	return NULL;
}

// std::string response = "CAP * LS :\r\n";
// send(newsockfd, response.c_str(), response.size(), 0);
// recv(newsockfd, buff, 100000, 0);
// std::cout << buff;