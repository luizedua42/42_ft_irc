/**======================
*            42sp
* @file      : main.c
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 16/04/2024
*========================**/

#include "../include/includes.hpp"
#include "../include/Server.hpp"

int main(int ac, char **av){
	if (ac != 3){
		std::cout << "Usage: " << av[0] << " <port> <password>"  << std::endl;
		return 1;
	}
	Server server;
	server.setPort(av[1]);
	server.setPassword(av[2]);

	std::cout << "Port: " << server.getPort() << std::endl;
	std::cout << "Password: " << server.getPassword() << std::endl;
	server.socketInit();
	std::cout << "Server is running..." << std::endl;
	server.receive();
	std::cout << "Server is closing..." << std::endl;
	return 0;
}