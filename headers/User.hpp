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
# include "mainHeader.hpp"

// class Channel;

class User {
	
	private:
		int			_userFD;
		std::string	_userIP;
		std::string	_nickName;
		std::string	_realName;
		std::string _userName;
		std::string _password;
		bool		_isAuth;

	public:
		//Getters and Setters
		std::string	clientBuff;
		void setuserFD(int userFD);
		void setRealName(std::string realName);
		void setNickName(std::string nickName);
		void setuserIP(char * userIP);
		
		int			getuserFD();
		std::string	getRealName();
		std::string	getNickName();
		std::string getUserName();
		std::string	getuserIP();
		std::string	getPassword();
		void		setPassword(std::string password);
		void 		setIsAuth(bool isAuth);
		bool		getIsAuth();

		User(const char* nickname);
};

#endif