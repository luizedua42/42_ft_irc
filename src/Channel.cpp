#include "../include/Channel.hpp"

void Channel::promoteToOperator(const std::string UserNickname) {
    std::map<std::string, User*>::iterator it = _Users.find(UserNickname);
    if (it != _Users.end()) {
        _operators[UserNickname] = it->second;
        _Users.erase(it);
    }
}

void Channel::demoteFromOperator(const std::string UserNickname) {
    std::map<std::string, User*>::iterator it = _operators.find(UserNickname);
    if (it != _operators.end()) {
        _Users[UserNickname] = it->second;
        _operators.erase(it);
    }
}

std::map<std::string, User*> Channel::getOperators() const {
    return this->_operators;
}

std::map<std::string, User*> Channel::getNonOperators() const {
    return this->_Users;
}

std::string Channel::getName() const {
	return this->_name;
}

std::string Channel::getMode() const {
	return this->_mode;
}

std::string Channel::getPassword() const {
	return this->_password;
}

void Channel::setName(const std::string& name) {
	this->_name = name;
}

void Channel::addUser(User* User) {
	_Users.insert(std::make_pair(User->getNickName(), User));
}

Channel::Channel(const std::string& name) : _name(name) {
	_topic = "chat";
	_mode = "default";
	_password = "";
	_Users.clear();
	_operators.clear();
}

Channel::~Channel(void) {
	_Users.clear();
	_operators.clear();
}

// Function to list all users' nicknames
void Channel::listUsers() const {
	std::cout << "List of Users:\n";
	for (std::map<std::string, User*>::const_iterator it = _Users.begin(); it != _Users.end(); ++it) {
		std::cout << it->second->getNickName() << std::endl;
	}
}

// Function to list all operators' nicknames
void Channel::listOperators() const {
	std::cout << "List of Operators:\n";
	for (std::map<std::string, User*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it) {
		std::cout << it->second->getNickName() << std::endl;
	}
}

void Channel::setMode(const std::string& mode) {
	_mode = mode;
}

void Channel::setPassword(std::string& password) {
	_password = password;
}

