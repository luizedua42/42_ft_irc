#include "../../headers/mainHeader.hpp"

void Server::nick(std::vector<std::string> options, int userFD) {
	User* user = Server::getUserByFD(userFD);
	if(user == NULL)
		return;

	std::string response;
	std::string nickname = options[0].substr(0, options[0].find('\r'));
	
	user->setNickName(nickname);

	if(user->isAuth() == true) {
		response = IRC + RPL_WELCOMENBR + user->getNickName() + RPL_WELCOME + user->getNickName() + "!" + user->getUserName() + "@*" + END;
		send(userFD, response.c_str(), response.size(), 0);
	}
}

