#include "../include/Channel.hpp"

void initModes(std::map<std::string, bool>& modes) {
	modes["o"] = false;
	modes["t"] = false;
	modes["i"] = false;
	modes["k"] = false;
	modes["l"] = false;
}

void Channel::promoteToOperator(const std::string UserNickname) {
    std::map<std::string, User*>::iterator it = _users.find(UserNickname);
    if (it != _users.end()) {
        _operators[UserNickname] = it->second;
        _users.erase(it);
    }
}

void Channel::demoteFromOperator(const std::string UserNickname) {
    std::map<std::string, User*>::iterator it = _operators.find(UserNickname);
    if (it != _operators.end()) {
        _users[UserNickname] = it->second;
        _operators.erase(it);
    }
}

std::map<std::string, User*> Channel::getOperators() const {
    return this->_operators;
}

std::map<std::string, User*> Channel::getNonOperators() const {
    return this->_users;
}

std::vector<User*> Channel::getAllUsers() const {
	std::vector<User*> allUsers;
	
	for (std::map<std::string, User*>::const_iterator it = _operators.begin(); it != _operators.end(); it++) {
		allUsers.push_back(it->second);
	}
	for (std::map<std::string, User*>::const_iterator it = _users.begin(); it != _users.end(); it++) {
		allUsers.push_back(it->second);
	}
	
	return allUsers;
}

std::string Channel::getName() const {
	return this->_name;
}

bool Channel::getModes(std::string mode) const {
	return this->_modes.at(mode);
}

std::string Channel::getPassword() const {
	return this->_password;
}

void Channel::setName(const std::string& name) {
	this->_name = name;
}

void Channel::addUser(User* User) {
	_users.insert(std::make_pair(User->getNickName(), User));
}

void Channel::removeUser(const std::string& nickname) {
    std::map<std::string, User*>::iterator it = _users.find(nickname);

    if (it != _users.end()) {
        _users.erase(it);
    } else {
        throw std::runtime_error(ERRMSG_NOTONCHANNEL);
    }
}

Channel::Channel(const std::string& name) : _name(name) {
	_topic = "";
	initModes(_modes);
	_password = "";
	_userLimit = MAX_USERS;
	_users.clear();
	_operators.clear();
}

Channel::~Channel(void) {
	_users.clear();
	_operators.clear();
}

// Function to list all users' nicknames
void Channel::listUsers() const {
	std::cout << "List of Users:\n";
	for (std::map<std::string, User*>::const_iterator it = _users.begin(); it != _users.end(); it++) {
		std::cout << it->second->getNickName() << std::endl;
	}
}

// Function to list all operators' nicknames
void Channel::listOperators() const {
	std::cout << "List of Operators:\n";
	for (std::map<std::string, User*>::const_iterator it = _operators.begin(); it != _operators.end(); it++) {
		std::cout << it->second->getNickName() << std::endl;
	}
}

void Channel::setMode(std::string mode, bool value) {
	_modes[mode] = value;
}

void Channel::setPassword(std::string& password) {
	_password = password;
}

void Channel::setTopic(const std::string& topic) {
	_topic = topic;
}

std::string Channel::getTopic() const {
	return this->_topic;
}

void Channel::setUserLimit(int limit) {
	_userLimit = limit;
}

int Channel::getUserLimit() const {
	return _userLimit;
}

bool Channel::isOperator(std::string userNickname) const {
	return _operators.find(userNickname) != _operators.end();
}
