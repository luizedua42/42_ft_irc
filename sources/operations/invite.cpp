#include "../../headers/mainHeader.hpp"

void Server::invite(const std::vector<std::string> options, int userFD) {
	std::string response;
	User* user = getUserByFD(userFD);

    if (options.size() < 2) {
		response = IRC + ERR_NEEDMOREPARAMSNBR + " INVITE " + ERR_NEEDMOREPARAMS + END;
		return;
    }

    const std::string& channelName = options[1];
    Channel* channelPtr = getChannel(channelName);

    if (channelPtr == NULL) {
        response = IRC + ERR_NOSUCHCHANNELNBR + channelName + ERR_NOSUCHCHANNEL + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

    if (!channelPtr->isUserOnChannel(user->getNickName())) {
		response = IRC + ERR_NOTONCHANNELNBR + channelName + ERR_NOTONCHANNEL + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

	if (!channelPtr->isUserOperator(user->getNickName())) {
		response = IRC + ERR_CHANOPRIVSNEEDEDNBR + channelName + ERR_NOTONCHANNEL + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}

	std::istringstream iss(options[0]);
	std::string invitedUserNick;
	while (std::getline(iss, invitedUserNick, ',')) {
		User* invitedUser = getUserByNick(invitedUserNick);
		if (invitedUser == NULL) {
			response = IRC + ERR_NOSUCHNICKNBR + invitedUserNick + ERR_NOSUCHNICK + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}

		if (channelPtr->isUserOnChannel(invitedUserNick))
		{
			response = IRC + ERR_USERONCHANNELNBR + invitedUserNick + " " + channelName + ERR_USERONCHANNEL + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}

		std::string response = IRC + RPL_INVITINGNBR + channelName + " " + user->getNickName() + RPL_INVITING + END;
		send(userFD, response.c_str(), response.size(), 0);
	}
}
