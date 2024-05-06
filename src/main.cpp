/**======================
*            42sp
* @file      : main.c
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 16/04/2024
*========================**/

#include "../include/includes.hpp"

// ./ircserv <port> <password>

int main(int ac, char **av) {
	if (ac != 3){
		std::cout << "Usage: " << av[0] << " <port> <password>"  << std::endl;
		return 1;
	}
	Server server;
	server.setPort(av[1]);
	server.setPassword(av[2]);
	
	std::cout << "Port: " << server.getPort() << std::endl;
	std::cout << "Password: " << server.getPassword() << std::endl;
	std::cout << "Server is running..." << std::endl;
    Channel channel("ChannelName");
	User user1("User1");
	User user2("User2");
	User user3("User3");
	User user4("User4");
	User user5("User5");

    // Add 5 users
    channel.addUser(&user1);
    channel.addUser(&user2);
    channel.addUser(&user3);
    channel.addUser(&user4);
    channel.addUser(&user5);

    // Promote 3 users to operators
    channel.promoteToOperator("User1");
    channel.promoteToOperator("User2");
    channel.promoteToOperator("User3");

    // Demote one operator back to a user
    channel.demoteFromOperator("User1");

    // List all users and operators
    std::cout << "All Users:\n";
    channel.listUsers();
    std::cout << "\nAll Operators:\n";
    channel.listOperators();

	try {
		signal(SIGINT, server.handleSig);
		signal(SIGQUIT, server.handleSig);
		server.setupServer();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}


	std::cout << "Server is closing..." << std::endl;
	return 0;
}

// int main(void){
// 	FakeChannel	channel;
// 	FakeUser	user1;
// 	FakeUser	user2;
// 	FakeUser	user3;
// 	FakeUser	user4;
// 	user1._name = "alo";
// 	user1._id = 1;
// 	user2._name = "bagre";
// 	user2._id = 2;
// 	user3._name = "casa";
// 	user3._id = 3;
// 	user4._name = "dog";
// 	user4._id = 4;
// 	channel._operators.push_back(user1);
// 	channel._operators.push_back(user2);
// 	// channel._operators.push_back(user3);
// 	channel._operators.push_back(user4);
// 	channel._users.push_back(user3);
// 	// std::cout << User: << user._name << " n:" << user._id << std::endl << std::cout;
// 	std::cout << User: << channel._operators[0]._name << " \tn:" << channel._operators[0]._id << std::endl <<
// 	User: << channel._operators[1]._name << " \tn:" << channel._operators[1]._id << std::endl <<
// 	User: << channel._operators[2]._name << " \tn:" << channel._operators[2]._id << std::endl <<
// 	User: << channel._operators[3]._name << " \tn:" << channel._operators[3]._id << std::endl <<
// 	User: << channel._users[0]._name << " \tn:" << channel._users[0]._id << std::endl;
// 	return 0;
// }