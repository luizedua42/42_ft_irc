#include "../../headers/mainHeader.hpp"

namespace mode {
	void setOp(Channel* channel, std::string user) {
		channel->promoteToOperator(user);
	}
	void unsetOp(Channel* channel, std::string user) {
		channel->demoteFromOperator(user);
	}
	void setTopic(Channel* channel) {
		channel->setMode("t", true);
	}
	void unsetTopic(Channel* channel) {
		channel->setMode("t", false);
	}
	void setInvite(Channel* channel) {
		channel->setMode("i", true);
	}
	void unsetInvite(Channel* channel) {
		channel->setMode("i", false);
	}
	void setKey(Channel* channel, std::string key) {
		channel->setPassword(key);
	}
	void unsetKey(Channel* channel) {
		std::string key = "";
		channel->setPassword(key);
	}
	void setLimit(Channel* channel, std::string param) {
		std::stringstream ss(param);
		int limit;
		ss >> limit;
		channel->setUserLimit(limit);
	}
	void unsetLimit(Channel* channel) {
		channel->setUserLimit(MAX_USERS);
	}
}

void Server::mode(std::vector<std::string> options, int clientFd) {
	std::string response;
	std::string mode;
	if(options.size() == 1) {
		std::cout << "/mode handle" << std::endl;
		return;
	}
	
	User* user = getUserByFD(clientFd);
	std::string channelName = options[0];
	Channel* channel = getChannel(channelName);
	int i = 0;
	std::string modes[] = {"-i", "+i", "-t", "+t", "-k", "+k", "-o", "+o", "-l", "+l"};
	mode = options[1].substr(0, options[1].find('\r'));
	std::string modeParam = "";
	if(options.size() > 3)
		modeParam = options[2];

	response = ":" + user->getNickName() + "!" + user->getUserName() + "@ft.irc MODE " + channelName + " ";

	for(; i < 10; i++) {
		if(mode == modes[i])
			break;
	}

	switch (i) {
		case 0:
			mode::unsetInvite(channel);
			response += "-i";
			break;
		case 1:
			mode::setInvite(channel);
			response += "+i";
			break;
		case 2:
			mode::unsetTopic(channel);
			response += "-t";
			break;
		case 3:
			mode::setTopic(channel);
			response += "+t";
			break;
		case 4:
			response += "-k";
			mode::unsetKey(channel);
			break;
		case 5:
			response += "+k";
			mode::setKey(channel, modeParam);
			break;
		case 6:
			mode::unsetOp(channel, modeParam);
			response += "-o";
			break;
		case 7:
			mode::setOp(channel, modeParam);
			response += "+o";
			break;
		case 8:
			mode::unsetLimit(channel);
			response += "-l";
			break;
		case 9:
			mode::setLimit(channel, modeParam);
			response += "+l";
			break;
		default:
			response = "Invalid mode";
			break;
	}
	response += " " + modeParam + END;
	std::cout << "RESPONSE::" << response << std::endl;
	send(clientFd, response.c_str(), response.size(), 0);
}
