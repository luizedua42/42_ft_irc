#include "../headers/mainHeader.hpp"

void Server::pass(std::vector<std::string> options, int userFD) {
	User* user = Server::getUser(userFD);
	std::string password = options[0].substr(0, options[0].find('\r'));

	if(user->getPassword().empty())
		user->setPassword(password);

	if (user->getPassword() != Server::getPassword()) {
		std::string response = ":ft.irc 464 :Password incorrect\r\n";
		std::cout << "Sending response: " << response << std::endl;
		send(userFD, response.c_str(), response.size(), 0);
		close(userFD);
		Server::clearUsers(userFD);
		return;
	} else if (user->getPassword() == Server::getPassword()){
		user->setIsAuth(true);
		std::string response = ":ft.irc 001 " + user->getNickName() + " :Welcome to the Internet Relay Chat " + user->getNickName() + "!" + user->getRealName() + "@*\r\n";
		std::cout << "Sending response: " << response << std::endl;
		send(userFD, response.c_str(), response.size(), 0);
	
	}
}
