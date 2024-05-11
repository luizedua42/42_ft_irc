#include "../headers/mainHeader.hpp"

void Server::pass(std::vector<std::string> options, int userFD) {
	User* user = Server::getUserByFD(userFD);
	std::string response;
	std::string password = options[0].substr(0, options[0].find('\r'));

	if(user->getPassword().empty())
		user->setPassword(password);

	if (user->getPassword() != Server::getPassword()) {
		response = IRC + ERR_PASSWDMISMATCHNBR + ERR_PASSWDMISMATCH + END;;
		send(userFD, response.c_str(), response.size(), 0);
		close(userFD);
		Server::clearUsers(userFD);
		return;
	} else if (user->getPassword() == Server::getPassword()){
		user->setIsAuth(true);
		response = IRC + RPL_WELCOMENBR + user->getNickName() + RPL_WELCOME + user->getNickName() + "!" + user->getRealName() + "@*" + END;
		send(userFD, response.c_str(), response.size(), 0);
	
	}
}
