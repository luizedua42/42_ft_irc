/**======================
*            42sp
* @file      : Server.hpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 17/04/2024
*========================**/
#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP
# include "includes.hpp"

class Server{
	private:
		int					_port;
		int					_sockfd;
		std::string			_password;
		struct sockaddr_in	_serv_addr;

	public:
		void		receive();
		void		parseOptions(std::string buff);

		void		setupSocket();
		void		setupServer(char **input);

		int			getPort();
		void		setPort(char *input);
		void		setPassword(char *input);
		std::string	getPassword();

		//Commands
		void		join(std::string channel);
		void		part(std::string channel);
		void		names(std::string channel);
		void		list(std::string channel);
		void		privmsg(std::string channel);
		void		quit(std::string channel);
		void		nick(std::string nickname);
		void		user(std::string username);
		void		oper(std::string user);
		void		mode(std::string channel);
};

#endif