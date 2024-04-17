/**======================
*            42sp
* @file      : Server.hpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 17/04/2024
*========================**/

#ifndef SERVER_HPP
# define SERVER_HPP
# include "includes.hpp"

class Server{
	private:
		int _port;
		int _sockfd;
		std::string _password;
		struct sockaddr_in _serv_addr;
	public:
		int		getPort();
		void	setPort(char *input);
		void	socketInit();
		void	receive();
		void	setPassword(char *input);
		std::string getPassword();
};

#endif