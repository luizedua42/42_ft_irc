/**======================
*            42sp
* @file      : Client.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 24/04/2024
*========================**/

#include "../include/includes.hpp"


//Getters and Setters

void Client::setClientFD(int clientFD) {
	_clientFD = clientFD;
}

void Client::setRealName(std::string realName) {
	_realName = realName;
}

void Client::setNickName(std::string nickName) {
	_nickName = nickName;
}

void Client::setClientIP(char *clientIP) {
	_clientIP = clientIP;
}

int Client::getClientFD() {
	return _clientFD;
}

std::string Client::getRealName() {
	return _realName;
}

std::string Client::getNickName() {
	return _nickName;
}

std::string Client::getClientIP() {
	return _clientIP;
}

Client::Client(const char* nickName) : _nickName(nickName) {}
