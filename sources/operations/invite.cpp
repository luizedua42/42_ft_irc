#include "../headers/mainHeader.hpp"

void Server::invite(const std::vector<std::string> options, int userFD) {
	(void)userFD;
	(void)options;
	// User* user = getUser(userFD);
    // if (options.size() < 2) {
	// 	throw std::runtime_error(ERR_NEEDMOREPARAMS);
    // }

    // const std::string& invitedUser = options[0];
    // const std::string& channelName = options[1];

    // Channel* channelPtr = getChannel(channelName);
    // if (channelPtr == NULL) {
    //     throw std::runtime_error(ERR_NOSUCHCHANNEL);
    // }

    // if (!channelPtr->isUserInChannel(user)) {
	// 	throw std::runtime_error(ERR_NOTONCHANNEL);
    // }

    // std::string response = ":ft.irc" + " 341 " + user.getNickName() + " " + invitedUser + " :" + channelName + END;
	// send(userFD, response.c_str(), response.size(), 0);
}
