#include "../headers/mainHeader.hpp"

void Server::nick(std::vector<std::string> options, int userFD) {
	User* user = Server::getUser(userFD);
	if(user == NULL)
		return;

	std::string nickname = options[0].substr(0, options[0].find('\r'));

	user->setNickName(nickname);
	if(user->getIsAuth() == true) {
		std::string response;
		response = ":ft.irc 001 " + user->getNickName() + " :Welcome to the Internet Relay Chat " + user->getNickName() + "!" + user->getRealName() + "@*\r\n";
		send(userFD, response.c_str(), response.size(), 0);
	}
}
