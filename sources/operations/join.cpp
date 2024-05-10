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

	std::vector<std::string> channels;
    splitString(options[0], ',', channels);

    std::vector<std::string> passwords;
    if (options.size() > 1) {
        splitString(options[1], ',', passwords);
    }

	User* user = getUser(userFD);
	for (size_t i = 0; i < channels.size(); i++) {
		if (!isValidChannelName(channels[i])) {
			std::cout << "invalid channel name message check" << std::endl;
			//send error invalid channel name
			continue;
		}

		if (!channelExists(channels[i])) {
			createChannel(channels[i]);
			isOperator = true;
		}

		Channel* channelPtr = getChannel(channels[i]);
		if (channelPtr->getModes("i") == true && !channelPtr->isUserInvited(user->getNickName())) {
			// send (ERRMSG_INVITEONLY);
			continue;
		}

		std::string password = channelPtr->getPassword();
		if (!password.empty() && password != passwords[i]) {
			//send error message invalid password
			continue;
		}

		channelPtr->addUser(user);
		if (isOperator) {
			channelPtr->promoteToOperator(user->getNickName());
		}
		
		std::cout << " Joining channel: " << channels[i] << std::endl;
		std::string response = ":" + user->getNickName() + " JOIN " + channels[i] + "\r\n";
		send(userFD, response.c_str(), response.size(), 0);
	}
}