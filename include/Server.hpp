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

class Oper;
class Server{
	private:
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
		int				getPort();
		void			setPort(char *input);
		void			setPassword(char *input);
		std::string		getPassword();
		Client&	getClient(int clientFd);
		std::vector<Client>	getClientVector();

		//Commands
		
		void						selectOptions(std::string buff, int clientFd);
		std::vector<std::string>	parseOptions(std::string buff);

		void		cap(int clientFd);
		void		join(std::vector<std::string>, int fd);
		void		privmsg(std::vector<std::string>, int fd);
		void		quit(std::vector<std::string>, int fd);
		void		nick(std::vector<std::string>, int fd);
		void		user(std::vector<std::string>, int fd);
		void		oper(std::vector<std::string>, int fd);
		void		mode(std::vector<std::string>, int fd);
		void		topic(std::vector<std::string>, int fd);
		void		invite(std::vector<std::string>, int fd);
		void		kick(std::vector<std::string>, int fd);
};

#endif