#include "../include/Channel.hpp"

void Channel::promoteToOperator(const std::string clientNickname) {
    std::map<std::string, Client*>::iterator it = _clients.find(clientNickname);
    if (it != _clients.end()) {
        _operators[clientNickname] = it->second;
        _clients.erase(it);
    }
}

void Channel::demoteFromOperator(const std::string clientNickname) {
    std::map<std::string, Client*>::iterator it = _operators.find(clientNickname);
    if (it != _operators.end()) {
        _clients[clientNickname] = it->second;
        _operators.erase(it);
    }
}

std::map<std::string, Client*> Channel::getOperators() {
    return this->_operators;
}

std::map<std::string, Client*> Channel::getNonOperators() {
    return this->_clients;
}

void Channel::addClient(Client* client) {
	_clients.insert(std::make_pair(client->getNickName(), client));
}

Channel::Channel(const char* name) : _name(name) {
	_topic = "chat";
	_mode = "default";
	_clients.clear();
	_operators.clear();
}

Channel::Channel(const char* name, std::string topic) : _name(name), _topic(topic) {
	_clients.clear();
	_operators.clear();
}

Channel::~Channel(void) {
	_clients.clear();
	_operators.clear();
}

// Function to list all users' nicknames
void Channel::listUsers() const {
	std::cout << "List of Users:\n";
	for (std::map<std::string, Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		std::cout << it->second->getNickName() << std::endl;
	}
}

// Function to list all operators' nicknames
void Channel::listOperators() const {
	std::cout << "List of Operators:\n";
	for (std::map<std::string, Client*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it) {
		std::cout << it->second->getNickName() << std::endl;
	}
}