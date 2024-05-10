#include "../headers/mainHeader.hpp"

void Server::part(const std::vector<std::string> options, int userFD) {
	User* user = getUser(userFD);
	std::string partReason = "";

    if (options.empty()) {
		//response with (ERR_NEEDMOREPARAMS);
		return;
    } else if (options.size() > 1) {
		for (size_t i = 1; i < options.size(); i++) {
			partReason += options[i];
		}
	}

    std::istringstream iss(options[0]);
    std::string channelName;

	while (std::getline(iss, channelName, ',')) {
		Channel* channelPtr = getChannel(channelName);
		if (channelPtr == NULL) {
			//response with (ERR_NOSUCHCHANNEL);
			std::string response = ":ft.irc 403 " + channelName + " :No such channel\r\n";
			// std::cout << "Sending response: " << response << std::endl;
			send(userFD, response.c_str(), response.size(), 0);
		} else {
			channelPtr->removeUser(user->getNickName());
			std::string response = ":" + user->getNickName() + "!~" + user->getRealName() + "@* PART " + channelName + " :" + partReason + "\r\n";
			std::vector<User *> users = channelPtr->getAllUsers();
			for (size_t i = 0; i < users.size(); i++) {
				send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
			}
		}
	}
}
