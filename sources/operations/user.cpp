#include "../../headers/mainHeader.hpp"

void Server::user(std::vector<std::string> options, int userFD) {
	User* user = Server::getUserByFD(userFD);
	if(user == NULL)
		return;
	std::string response;
	std::string username = options[0].substr(0, options[0].find(' '));
	if (!user->getUserName().empty()){
		response =	IRC + ERR_ALREADYREGISTEREDNBR + user->getNickName() + ERR_ALREADYREGISTERED + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}
	user->setUserName(username);
	if (options.size() > 3) 
		user->setRealName(options[3].substr(1, options[3].find('\r')));
	
	if(user->isAuth() == true) {
		response = IRC + RPL_WELCOMENBR + user->getNickName() + RPL_WELCOME + user->getNickName() + "!" + user->getUserName() + "@*" + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}
}