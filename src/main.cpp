/**======================
*            42sp
* @file      : main.c
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 16/04/2024
*========================**/

#include "../include/includes.hpp"
#include <sstream>

int main(int ac, char **av){
	std::stringstream iss;
	std::string password;
	if (ac != 3){
		std::cout << "Usage: " << av[0] << " <port> <password>"  << std::endl;
		return 1;
	}
	iss << av[1];
	int port;
	iss >> port;
	if(port > 65535 || port < 0){
		std::cout << "Invalid port number" << std::endl;
		return 1;
	}
	std::cout << "Port: " << port << std::endl;
	password = av[2];
	std::cout << "Password: " << password << std::endl;
	return 0;
}