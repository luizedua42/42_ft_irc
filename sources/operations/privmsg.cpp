#include "../headers/mainHeader.hpp"

std::string messageCat(std::vector<std::string> options) {
	std::string message;
	if (options[1].find(':') == 0) {
		message = options[1].substr(1) + " ";
		for (size_t i = 2; i < options.size(); i++) {
			message += options[i] + " ";
		}
		return message;
	}
	message = options[1].substr(0) + " ";
	for (size_t i = 2; i < options.size(); i++) {
		message += options[i] + " ";
	}
	return message;
}

void Server::privmsg(std::vector<std::string> options, int userFD) {
	std::string channelName = options[0].substr(0, options[0].find(' '));
	std::cout << "Sending message to channel: " << channelName << std::endl;
	std::string message = messageCat(options);
	std::cout << "Message: " << message << std::endl;
	User* user = Server::getUser(userFD);

	Channel* channelPtr = getChannel(channelName);
	if (channelPtr == NULL && channelName[0] == '#') {
		std::string response = ":ft.irc 401 " + channelName + " :No such nick/channel\r\n";
		std::cout << "Sending response: " << response << std::endl;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	} else if (channelName[0] != '#'){
		User* recipient = searchUser(channelName);
		if (recipient == NULL) {
			std::string response = ":ft.irc 401 " + channelName + " :No such nick/channel\r\n";
			std::cout << "Sending response: " << response << std::endl;
			send(userFD, response.c_str(), response.size(), 0);
			return;
		}
		std::string response = ":" + user->getNickName() + " PRIVMSG " + recipient->getNickName() + " :" + message + "\r\n";
		std::cout << "Sending response: " << response << std::endl;
		send(recipient->getuserFD(), response.c_str(), response.size(), 0);
		return;
	}

	std::string response = ":" + user->getNickName() + " PRIVMSG " + channelName + " :" + message + "\r\n";
	std::cout << "Sending response: " << response << std::endl;
	std::map<std::string, User*> users = channelPtr->getNonOperators();
	for (std::map<std::string, User*>::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->second->getNickName() != user->getNickName()) {
			if(it->second->getuserFD() != userFD)
				send(it->second->getuserFD(), response.c_str(), response.size(), 0);
		}
	}
	std::map<std::string, User*> operators = channelPtr->getOperators();
	for (std::map<std::string, User*>::iterator it = operators.begin(); it != operators.end(); ++it) {
		if (it->second->getNickName() != user->getNickName()) {
			if(it->second->getuserFD() != userFD)
				send(it->second->getuserFD(), response.c_str(), response.size(), 0);
		}
	}
}
