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

class Server {
	private:
		int							_port;
		int							_sockfd;
		static bool					_signal;
		std::string					_password;
		std::vector<Channel>		_channels;
		std::vector<User>			_Users;
		std::vector<struct pollfd>	_fds;

	public:
		//Methods
		void						listenUser(int UserFD);
		void						acceptNewUser(const char* nickName);
		void						clearUsers(int UserFd);
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
		User&	getUser(int UserFd);
		std::vector<User>	getUserVector();

		//Commands
		
		void						selectOptions(std::string buff, int UserFd);
		std::vector<std::string>	parseOptions(std::string buff);

		void		cap(int UserFd);
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