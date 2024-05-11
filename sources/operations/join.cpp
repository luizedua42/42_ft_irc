#include "../headers/mainHeader.hpp"

static bool isValidChannelName(const std::string& channelName) {
    if (channelName.empty() || channelName[0] != '#') {
        return false;
    }

    for (size_t i = 1; i < channelName.size(); i++) {
        if (!isalnum(channelName[i]) && channelName[i] != '-' && channelName[i] != '_' && channelName[i] != '+') {
            return false;
        }
    }

    return true;
}

void Server::join(std::vector<std::string> options, int userFD) {
	bool isOperator = false;

	std::string response;
	std::vector<std::string> channels;
    splitString(options[0], ',', channels);

    std::vector<std::string> passwords;
    if (options.size() > 1) {
        splitString(options[1], ',', passwords);
    }

	User* user = getUser(userFD);
	for (size_t i = 0; i < channels.size(); i++) {
		if (!isValidChannelName(channels[i])) {
			response = IRC + ERR_BADCHANMASKNBR + channels[i] + ERR_BADCHANMASK + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}

		if (!channelExists(channels[i])) {
			createChannel(channels[i]);
			isOperator = true;
		}

		Channel* channelPtr = getChannel(channels[i]);
		if (channelPtr->getModes("i") == true && !channelPtr->isUserInvited(user->getNickName())) {
			response = IRC + ERR_INVITEONLYCHANNBR + channels[i] + ERR_INVITEONLYCHAN + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}

		std::string password = channelPtr->getPassword();
		if (!password.empty() && password != passwords[i]) {
			response = IRC + ERR_PASSWDMISMATCHNBR + channels[i] + ERR_PASSWDMISMATCH + END;
			send(userFD, response.c_str(), response.size(), 0);
			continue;
		}

		channelPtr->addUser(user);
		if (isOperator) {
			channelPtr->promoteToOperator(user->getNickName());
		}
		
		std::cout << " Joining channel: " << channels[i] << std::endl;
		std::string response = ":" + user->getNickName() + " JOIN " + channels[i] + END;
		send(userFD, response.c_str(), response.size(), 0);
	}
}
