#include "../headers/mainHeader.hpp"

void Server::quit(std::vector<std::string> options, int userFD) {
	std::string channel= options[0];
	User* user = Server::getUserByFD(userFD);
	
	std::cout << user->getNickName() << ":" << "Quittin" << channel << std::endl;
}
