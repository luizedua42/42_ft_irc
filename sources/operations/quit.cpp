#include "../headers/mainHeader.hpp"

void Server::quit(std::vector<std::string> options, int userFD) {
	std::string channel= options[0];
	User* test = Server::getUser(userFD);
	
	std::cout << test->getNickName() <<":" <<"Quittin" << channel << std::endl;
}
