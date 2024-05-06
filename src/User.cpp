/**======================
*            42sp
* @file      : User.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 24/04/2024
*========================**/

#include "../include/includes.hpp"


//Getters and Setters

void User::setUserFD(int userFD) {
	_UserFD = userFD;
}

void User::setRealName(std::string realName) {
	_realName = realName;
}

void User::setNickName(std::string nickName) {
	_nickName = nickName;
}

void User::setUserIP(char *userIP) {
	_UserIP = userIP;
}

int User::getUserFD() {
	return _UserFD;
}

std::string User::getRealName() {
	return _realName;
}

std::string User::getNickName() {
	return _nickName;
}

std::string User::getUserIP() {
	return _UserIP;
}

User::User(const char* nickName) : _nickName(nickName) {}
