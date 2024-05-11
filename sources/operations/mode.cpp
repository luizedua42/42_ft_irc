#include "../headers/mainHeader.hpp"

namespace mode {
	void setOp(Channel* channel, std::string user) {
		std::cout << "Setting operator: " << user << std::endl;
		channel->promoteToOperator(user);
	}
	void unsetOp(Channel* channel, std::string user) {
		std::cout << "Unsetting operator: " << user << std::endl;
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
	if(options.size() < 2 || options.size() > 3){
		std::cerr << "Invalid number of arguments" << std::endl;
		return;
	}

	std::string channelName = options[0];
	Channel* channel = getChannel(channelName);
	
	int i = 0;
	std::string modes[] = {"-i", "+i", "-t", "+t", "-k", "+k", "-o", "+o", "-l", "+l"};
	std::string mode = options[1].substr(0, options[1].find('\r'));
	std::string modeParam = "";
	if (options.size() == 3){
		modeParam = options[2].substr(0, options[2].find('\r'));
	}
	// std::cout << "User: " << user->getNickName() <<" Setting mode: " << mode << " in channel: " << channelName << ". Param: " << modeParam << std::endl;
	for(; i < 10; i++) {
		if(mode == modes[i])
			break;
	}
	switch (i) {
		case 0:
			mode::unsetInvite(channel);
			break;
		case 1:
			mode::setInvite(channel);
			break;
		case 2:
			mode::unsetTopic(channel);
			break;
		case 3:
			mode::setTopic(channel);
			break;
		case 4:
			mode::unsetKey(channel);
			break;
		case 5:
			mode::setKey(channel, modeParam);
			break;
		case 6:
			mode::unsetOp(channel, modeParam);
			break;
		case 7:
			mode::setOp(channel, modeParam);
			break;
		case 8:
			mode::unsetLimit(channel);
			break;
		case 9:
			mode::setLimit(channel, modeParam);
			break;
		default:
			Server::unknownCommand(mode, clientFd);
			break;
	}
}
