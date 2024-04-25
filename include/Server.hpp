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
		Oper						_serverOper;
		int							_port;
		int							_sockfd;
		static bool					_signal;
		std::string					_password;
		std::vector<Client>			_clients;
		std::vector<struct pollfd>	_fds;

	public:
		//Methods
		void						listenClient(int clientFD);
		void						acceptNewClient();
		void						clearClients(int clientFd);
		void						closeFds();
		static void					handleSig(int signum);
		//Setup
		void		setupSocket();
		void		setupServer();
		
		//Getters
		int			getPort();
		void		setPort(char *input);
		void		setPassword(char *input);
		std::string	getPassword();

		//Commands

};

#endif