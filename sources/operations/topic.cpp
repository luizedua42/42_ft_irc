#include "../../headers/mainHeader.hpp"

void Server::topic(std::vector<std::string> options, int clientFd) {
	std::string response;
	
	std::string channelName = options[0];
	
	Channel* channelPtr = getChannel(channelName);
	User* user = Server::getUserByFD(clientFd);

	if (options.size() == 1) {
		if(channelPtr->getTopic().size() == 0) {
			response = IRC + RPL_NOTOPICNBR + user->getNickName() + " " + channelName + " :No topic is set." + END;
			if(send (clientFd, response.c_str(), response.size(), 0) == -1)
				std::cerr << "Error sending message" << std::endl;
			return;
		}
		
		response = IRC + RPL_TOPICNBR + user->getNickName() + " " + channelName + " :" + channelPtr->getTopic() + END;
		send(clientFd, response.c_str(), response.size(), 0);
		return;
	} else if (options.size() > 1) {
		std::string topic = options[1].substr(1);

		if(channelPtr->getModes("t") == true && !channelPtr->isUserOperator(user->getNickName())) {
			response = IRC + ERR_CHANOPRIVSNEEDEDNBR + user->getNickName() + " " + channelName + ERR_CHANOPRIVSNEEDED + END;
			
			send(clientFd, response.c_str(), response.size(), 0);
			
			return;
		}

		channelPtr->setTopic(topic);
		
		std::vector<User *> users = channelPtr->getAllUsers();

		response = ":" + user->getNickName() + "!~" + user->getRealName() + "@ft.irc TOPIC " + channelName + " " + topic + END;
		
		for (size_t i = 0; i < users.size(); i++) {
			send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
		}
	} else {
		response = IRC + ERR_NEEDMOREPARAMSNBR + user->getNickName() + " "+ channelName +" TOPIC :Wrong number of parameters" + END;
		
		send(clientFd, response.c_str(), response.size(), 0);
	}
}
