#include "../headers/mainHeader.hpp"

void Server::topic(std::vector<std::string> options, int clientFd) {
	std::string notopic = " 331 ";
	std::string response;
	
	std::string channelName = options[0];
	
	Channel* channelPtr = getChannel(channelName);
	User* user = Server::getUser(clientFd);
	std::cout << "User: " << user->getNickName() << "userFD: " << clientFd << std::endl;
	if (options.size() == 1) {
		std::cout << "Getting topic of channel: " << channelName << std::endl;
		std::cout << std::boolalpha << channelPtr->getTopic().empty() << std::endl;
		if(channelPtr->getTopic().size() == 0) {
			std::cout << "Topic: " << '"' + channelPtr->getTopic() + '"' << std::endl;
			response = ":ft.irc" + notopic + user->getNickName() + " " + channelName + " :No topic is set.\r\n";
			std::cout << "Sending response: " << response << std::endl;
			//PROBLEM WITH THIS SEND, CLIENT DOES NOT GET THE RESPONSE
			if(send (clientFd, response.c_str(), response.size(), 0) == -1)
				std::cerr << "Error sending message" << std::endl;
			return;
		}
		response = ":ft.irc 332 " + user->getNickName() + " " + channelName + " :" + channelPtr->getTopic() + "\r\n";
		std::cout << "Sending response: " << response << std::endl;
		send(clientFd, response.c_str(), response.size(), 0);
		return;
	} else if (options.size() == 2 ) {
		std::string topic = options[1].substr(1);
		if(channelPtr->getModes("t") == true && !channelPtr->isOperator(user->getNickName())) {
			std::string response = ":ft.irc 482 " + user->getNickName() + " " + channelName + " :You're not channel operator\r\n";
			std::cout << "Sending response: " << response << std::endl;
			send(clientFd, response.c_str(), response.size(), 0);
			return;
		}
		std::cout << "Setting topic of channel: " << channelName << " to: " << topic << std::endl;
		channelPtr->setTopic(topic);
		std::vector<User *> users = channelPtr->getAllUsers();
		response = ":" + user->getNickName() + "!~" + user->getRealName() + "@ft.irc TOPIC " + channelName + " " + topic + "\r\n";
		for (size_t i = 0; i < users.size(); i++) {
			send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
		}
	} else {
		std::cerr << "Invalid number of arguments" << std::endl;
	}
}
