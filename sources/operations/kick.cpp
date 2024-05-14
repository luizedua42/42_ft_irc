#include "../../headers/mainHeader.hpp"

void Server::kick(std::vector<std::string> options, int userFD) {
	User* user = getUserByFD(userFD);
	std::string response;

    if (options.size() < 2) {
		response = IRC + ERR_NEEDMOREPARAMSNBR + " KICK " + ERR_NEEDMOREPARAMS + END;
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
		response = IRC + ERR_NOSUCHCHANNELNBR + channelName + ERR_NOSUCHCHANNEL + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

    if (!channelPtr->isUserOperator(user->getNickName())) {
		response = IRC + ERR_CHANOPRIVSNEEDEDNBR + user->getNickName() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

	std::vector<User *> users = channelPtr->getAllUsers();
	response = ":" + user->getNickName() + "!" + user->getRealName() + "@ft.irc KICK " + channelName + " " + userToBeKicked + " :"+ kickReason + END;
	for (size_t i = 0; i < users.size(); i++) {
		send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
	}
    channelPtr->removeUser(userToBeKicked);
}