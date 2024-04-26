/**======================
*            42sp
* @file      : Client.hpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 24/04/2024
*========================**/

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "includes.hpp"

class Channel;

class Client {
	
	private:
		int			_clientFD;
		std::string	_realName;
		std::string	_nickName;
		std::string	_clientIP;

	public:
		//Getters and Setters
		void setClientFD(int clientFD);
		void setRealName(std::string realName);
		void setNickName(std::string nickName);
		void setClientIP(char * clientIP);
		
		int			getClientFD();
		std::string	getRealName();
		std::string	getNickName();
		std::string	getClientIP();

		Client(const char* nickname);
};

#endif