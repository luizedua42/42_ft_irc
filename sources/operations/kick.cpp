#include "../headers/mainHeader.hpp"

void Server::kick(std::vector<std::string> options, int userFD) {
	User* user = getUser(userFD);

    if (options.size() < 2) {
		//response with (ERR_NEEDMOREPARAMS);
		return;
    }

	std::string kickReason;
    const std::string& channelName = options[0];
    const std::string& userToBeKicked = options[1];

	if (options.size() == 3) { 
		kickReason = options[2].substr(1);
	} else {
		kickReason = "no reason";
	}

    Channel* channelPtr = getChannel(channelName);
    if (channelPtr == NULL) {
        //response with (ERR_NOSUCHCHANNEL);
		std::string response = ":ft.irc 403 " + channelName + " :No such channel\r\n";
		// std::cout << "Sending response: " << response << std::endl;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

    if (!channelPtr->isOperator(user->getNickName())) {
        //response with (ERR_CHANOPRIVSNEEDED);
		std::string response = ":ft.irc 482 " + channelName + ":You are not a channel operator\r\n";
		// std::cout << "Sending response: " << response << std::endl;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

	std::vector<User *> users = channelPtr->getAllUsers();
	std::string response = ":" + user->getNickName() + "!" + user->getRealName() + "@ft.irc KICK " + channelName + " " + userToBeKicked + " :"+ kickReason + "\r\n";
	std::cout << "Sending response: " << response << std::endl;
	for (size_t i = 0; i < users.size(); i++) {
		send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
	}
    channelPtr->removeUser(userToBeKicked);
}
