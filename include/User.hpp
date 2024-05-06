/**======================
*            42sp
* @file      : User.hpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 24/04/2024
*========================**/
#pragma once
#ifndef User_HPP
# define User_HPP
# include "includes.hpp"

// class Channel;

class User {
	
	private:
		int			_UserFD;
		std::string	_realName;
		std::string	_nickName;
		std::string	_UserIP;

	public:
		//Getters and Setters
		std::string	clientBuff;
		void setUserFD(int userFD);
		void setRealName(std::string realName);
		void setNickName(std::string nickName);
		void setUserIP(char * UserIP);
		
		int			getUserFD();
		std::string	getRealName();
		std::string	getNickName();
		std::string	getUserIP();

		User(const char* nickname);
};

#endif