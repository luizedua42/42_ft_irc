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
		int							_port;
		int							_sockfd;
		std::string					_password;
		std::vector<struct pollfd>	_fds;

	public:
		//Methods
		void						listenClient(int clientFD);
		void						acceptNewClient();
		void						selectOptions(std::string buff);
		std::vector<std::string>	parseOptions(std::string buff);

		//Setup
		void		setupSocket();
		void		setupServer();
		
		//Getters
		int			getPort();
		void		setPort(char *input);
		void		setPassword(char *input);
		std::string	getPassword();

		//Commands
		void		join(std::vector<std::string>);
		void		privmsg(std::vector<std::string>);
		void		quit(std::vector<std::string>);
		void		nick(std::vector<std::string>);
		void		user(std::vector<std::string>);
		void		oper(std::vector<std::string>);
		void		mode(std::vector<std::string>);
		void		topic(std::vector<std::string>);
		void		invite(std::vector<std::string>);
		void		kick(std::vector<std::string>);
};

#endif