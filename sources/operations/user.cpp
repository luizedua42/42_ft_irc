#include "../headers/mainHeader.hpp"

void Server::user(std::vector<std::string> options, int userFD) {
	User* user = Server::getUserByFD(userFD);
	if(user == NULL)
		return;
	std::string username = options[0].substr(0, options[0].find(' '));

	user->setRealName(username);
}
