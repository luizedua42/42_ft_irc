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
	server.setupServer(av);

	std::cout << "Port: " << server.getPort() << std::endl;
	std::cout << "Password: " << server.getPassword() << std::endl;
	server.setupSocket();
	std::cout << "Server is running..." << std::endl;
	try{
		server.receive();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	std::cout << "Server is closing..." << std::endl;
	return 0;
}