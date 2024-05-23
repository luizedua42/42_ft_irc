#include "../../headers/mainHeader.hpp"

static std::vector<std::string>splitTopic(std::string str) {
	std::vector<std::string> result;
	size_t spacePos = str.find(' ');
	if (spacePos != std::string::npos) {
		std::string firstWord = str.substr(0, spacePos);
		//cutting "\r\n"
		std::string remaining = str.substr(spacePos + 1, str.find('\r') - spacePos - 1);
		result.push_back(firstWord);
		result.push_back(remaining);
	} else {
		result.push_back(str.substr(0,str.find("\r")));
	}
	return result;
}

void Server::topic(std::string options, int userFD) {
	std::string response;
	std::vector<std::string> splittedOptions = splitTopic(options);
	std::string channelName = splittedOptions[0];

	std::cout << "channelName: " << channelName << std::endl;
	if (channelName.empty()){
		response = IRC + ERR_NEEDMOREPARAMSNBR + " TOPIC " + ERR_NEEDMOREPARAMS + END;		
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}

	Channel* channelPtr = getChannel(channelName);

	if (channelPtr == NULL) {
		std::cout << "channelPtr is NULL" << std::endl;
		response = IRC + ERR_NOSUCHCHANNELNBR + channelName + ERR_NOSUCHCHANNEL + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}

	User* user = Server::getUserByFD(userFD);

	if (splittedOptions.size() == 1 || splittedOptions[1] == "") {

		if(channelPtr->getTopic().empty()) {
			response = IRC + RPL_NOTOPICNBR + user->getNickName() + " " + channelName + RPL_NOTOPIC + END;
			send (userFD, response.c_str(), response.size(), 0);
			return;
		}
		
		response = IRC + RPL_TOPICNBR + user->getNickName() + " " + channelName + " :" + channelPtr->getTopic() + END;
		send(userFD, response.c_str(), response.size(), 0);
		return;

	} else if (splittedOptions.size() > 1) {
		std::string topic = splittedOptions[1].substr(1);

		if(channelPtr->getMode("t") == true && !channelPtr->isUserOperator(user->getNickName())) {
			response = IRC + ERR_CHANOPRIVSNEEDEDNBR + user->getNickName() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
			send(userFD, response.c_str(), response.size(), 0);
			return;
		}

		channelPtr->setTopic(topic);
		
		std::vector<User *> users = channelPtr->getAllUsers();

		response = ":" + user->getNickName() + "!~" + user->getRealName() + "@ft.irc TOPIC " + channelName + " " + topic + END;
		
		for (size_t i = 0; i < users.size(); i++) {
			send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
		}
	}
}
