/**======================
*            42sp
* @file      : User.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 24/04/2024
*========================**/

#include "../headers/mainHeader.hpp"


//Getters and Setters

void User::setuserFD(int userFD) {
	_userFD = userFD;
}

void User::setRealName(std::string realName) {
	_realName = realName;
}

void User::setNickName(std::string nickName) {
	_nickName = nickName;
}

void User::setuserIP(char *userIP) {
	_userIP = userIP;
}

int User::getuserFD() {
	return _userFD;
}

std::string User::getRealName() {
	return _realName;
}

std::string User::getNickName() {
	return _nickName;
}

std::string User::getuserIP() {
	return _userIP;
}

User::User(const char* nickName) : _nickName(nickName) {
	_isAuth = false;
}

std::string User::getPassword() {
	return _password;
}

void User::setPassword(std::string password) {
	_password = password;
}

void User::setIsAuth(bool isAuth) {
	_isAuth = isAuth;
}

bool User::getIsAuth() {
	return _isAuth;
}
