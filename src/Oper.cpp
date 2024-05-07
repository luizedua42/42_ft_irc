/**======================
*            42sp
* @file      : Oper.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 25/04/2024
*========================**/

#include "../include/includes.hpp"
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
	void unknownCommand(std::string command, int userFD) {
		std::string response = ":ft.irc 421 " + command + ":Unknown command\r\n";
		//PROBLEM WITH THIS SEND, CLIENT DOES NOT GET THE RESPONSE
		if(send(userFD, response.c_str(), response.size(), 0) == -1)
			std::cerr << "Error sending message" << std::endl;
	}
}

std::vector<std::string> Server::parseOptions(std::string str) {
	std::string word;
	std::stringstream ss(str);
	std::vector<std::string> splitted;
	while (std::getline(ss, word, ' ')) {
		if(word.find('\r') != std::string::npos)
			splitted.push_back(word.substr(0, word.find('\r')));
		else
			splitted.push_back(word);
	}
	return splitted;
}

std::vector<std::string> splitBuff(std::string buff) {
	std::vector<std::string> splittedBuff;
	std::string word;
	std::stringstream ss(buff);
	while (std::getline(ss, word, '\n')) {
		splittedBuff.push_back(word);
	}
	return splittedBuff;
}

void Server::selectOptions(std::string buff, int userFD) {
	std::vector<std::string> splittedBuff = splitBuff(buff);
	
	std::string requests[] = {"CAP", "USER", "NICK", "JOIN", "PRIVMSG", "QUIT", "OPER", "MODE", "TOPIC", "INVITE", "KICK", "WHO"};
	
	do{
		int i = 0;
		std::string option = splittedBuff[0].substr(0, splittedBuff[0].find_first_of(" "));
		std::cout << "buff: " << buff << std::endl;
		for(; i < 12; i++) {
			if(option == requests[i])
				break;
		}

		std::string parsedOptions = splittedBuff[0].substr(splittedBuff[0].find_first_of(" ") + 1);
		std::cout << "option: " << option << std::endl;
		switch (i) {
			case 0:
				cap(userFD);
				break;
			case 1:
				user(parseOptions(parsedOptions), userFD);
				break;
			case 2:
				nick(parseOptions(parsedOptions), userFD);
				break;
			case 3:
				join(parseOptions(parsedOptions), userFD);
				break;
			case 4:
				privmsg(parseOptions(parsedOptions), userFD);
				break;
			case 5:
				quit(parseOptions(parsedOptions), userFD);
				break;
			case 6:
				oper(parseOptions(parsedOptions), userFD);
				break;
			case 7:
				mode(parseOptions(parsedOptions), userFD);
				break;
			case 8:
				topic(parseOptions(parsedOptions), userFD);
				break;
			case 9:
				invite(parseOptions(parsedOptions), userFD);
				break;
			case 10:
				kick(parseOptions(parsedOptions), userFD);
				break;
			case 11:
				who(parseOptions(parsedOptions), userFD);
				break;
			default:
				mode::unknownCommand(option, userFD);
				break;
		}
		splittedBuff.erase(splittedBuff.begin());
		} while (!splittedBuff.empty());
}

void Server::cap(int userFD) {
	(void)userFD;
	
	std::cout << "CAP" << std::endl;
}

void Server::join(std::vector<std::string> options, int userFD) {
	std::string channelName = options.front().substr(0);
	bool isOperator = false;

	if (!channelExists(channelName)) {
		createChannel(channelName);
		isOperator = true;
	}

	Channel* channel = getChannel(channelName);
	if (channel->getModes("i") == true) {
		throw std::runtime_error(ERRMSG_InviteOnly);
		return;
    }
	if (!channel->getPassword().empty()) {
        //ask_for password || return a msg asking to join with /join [channelName] [password]
		std::cout << "Password handling goes here";
        return;
    }

	User& user = Server::getUser(userFD);
	channel->addUser(&user);
	if (isOperator) {
		channel->promoteToOperator(user.getNickName());
	}
	
	std::cout << " Joining channel: " << channelName << std::endl;
	std::string response = ":" + user.getNickName() + " JOIN " + channelName + "\r\n";
	send(userFD, response.c_str(), response.size(), 0);
}

std::string messageCat(std::vector<std::string> options) {
	std::string message;
	message = options[1].substr(1) + " ";
	for (size_t i = 2; i < options.size(); i++) {
		message += options[i] + " ";
	}
	return message;
}

void Server::privmsg(std::vector<std::string> options, int userFD) {
	std::string channelName = options[0].substr(0, options[0].find(' '));
	std::cout << "Sending message to channel: " << channelName << " - " << std::endl;\
	std::string message = messageCat(options);
	std::cout << "Message: " << message << std::endl;

	Channel* channel = getChannel(channelName);
	if (channel == NULL) {
		std::cerr << "Channel not found" << std::endl;
		return;
	}

	User& user = Server::getUser(userFD);

	std::string response = ":" + user.getNickName() + " PRIVMSG " + channelName + " :" + message + "\r\n";
	std::cout << "Sending response: " << response << std::endl;
	std::map<std::string, User*> users = channel->getNonOperators();
	for (std::map<std::string, User*>::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->second->getNickName() != user.getNickName()) {
			if(it->second->getuserFD() != userFD)
				send(it->second->getuserFD(), response.c_str(), response.size(), 0);
		}
	}
	std::map<std::string, User*> operators = channel->getOperators();
	for (std::map<std::string, User*>::iterator it = operators.begin(); it != operators.end(); ++it) {
		if (it->second->getNickName() != user.getNickName()) {
			if(it->second->getuserFD() != userFD)
				send(it->second->getuserFD(), response.c_str(), response.size(), 0);
		}
	}

}

void Server::quit(std::vector<std::string> options, int userFD) {
	std::string channel= options[0];
	User& test = Server::getUser(userFD);
	
	std::cout << test.getNickName() <<":" <<"Quittin" << channel << std::endl;
}

void Server::nick(std::vector<std::string> option, int userFD) {
	User& user = Server::getUser(userFD);

	std::string nickname = option[0].substr(0, option[0].find('\r'));

	user.setNickName(nickname);

	std::string response;
	response = ":ft.irc 001 " + user.getNickName() + " :Welcome to the Internet Relay Chat " + user.getNickName() + "!" + user.getRealName() + "@*\r\n";
	send(userFD, response.c_str(), response.size(), 0);
}

void Server::user(std::vector<std::string> option, int userFD) {
	User& user = Server::getUser(userFD);

	std::string username = option[0].substr(0, option[0].find(' '));

	user.setRealName(username);
}


void Server::oper(std::vector<std::string> option, int userFD) {
	(void)userFD;
	std::cout << "Opering user: " << option[0] << std::endl;
}

void Server::mode(std::vector<std::string> option, int clientFd) {
	if(option.size() < 2 || option.size() > 3){
		std::cerr << "Invalid number of arguments" << std::endl;
		return;
	}

	std::string channelName = option[0];
	Channel* channel = getChannel(channelName);
	
	int i = 0;
	std::string modes[] = {"-i", "+i", "-t", "+t", "-k", "+k", "-o", "+o", "-l", "+l"};
	std::string mode = option[1].substr(0, option[1].find('\r'));
	std::string modeParam = option[2].substr(0, option[2].find('\r'));
	// std::cout << "User: " << user.getNickName() <<" Setting mode: " << mode << " in channel: " << channelName << ". Param: " << modeParam << std::endl;
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
			mode::unknownCommand(mode, clientFd);
			break;
	}
}

void Server::topic(std::vector<std::string> option, int clientFd) {
	std::string notopic = " 331 ";
	std::string response;
	
	std::string channelName = option[0];
	
	Channel* channel = getChannel(channelName);
	User& user = Server::getUser(clientFd);
	std::cout << "User: " << user.getNickName() << "userFD: " << clientFd << std::endl;
	if (option.size() == 1) {
		std::cout << "Getting topic of channel: " << channelName << std::endl;
		std::cout << std::boolalpha << channel->getTopic().empty() << std::endl;
		if(channel->getTopic().size() == 0) {
			std::cout << "Topic: " << '"' + channel->getTopic() + '"' << std::endl;
			response = ":ft.irc" + notopic + user.getNickName() + " " + channelName + " :No topic is set\r\n";
			std::cout << "Sending response: " << response << std::endl;
			//PROBLEM WITH THIS SEND, CLIENT DOES NOT GET THE RESPONSE
			if(send (clientFd, response.c_str(), response.size(), 0) == -1)
				std::cerr << "Error sending message" << std::endl;
			return;
		}
		response = ":ft.irc 332 " + user.getNickName() + " " + channelName + " :" + channel->getTopic() + "\r\n";
		std::cout << "Sending response: " << response << std::endl;
		send(clientFd, response.c_str(), response.size(), 0);
		return;
	} else if (option.size() == 2) {
		std::string topic = option[1].substr(1);
		std::cout << "Setting topic of channel: " << channelName << " to: " << topic << std::endl;
		channel->setTopic(topic);
	} else {
		std::cerr << "Invalid number of arguments" << std::endl;
	}
}

void Server::invite(std::vector<std::string> option, int userFD) {
	(void)userFD;
	std::cout << "Inviting user to channel: " << option[0] << std::endl;
}

void Server::kick(std::vector<std::string> option, int userFD) {
	(void)option;
	(void)userFD;
}
static std::vector<User *> getChannelUsers(Channel* channel) {
	std::vector<User *> users;
	std::map<std::string, User*> nonOps = channel->getNonOperators();
	for (std::map<std::string, User*>::iterator it = nonOps.begin(); it != nonOps.end(); ++it) {
		users.push_back(it->second);
	}
	std::map<std::string, User*> ops = channel->getOperators();
	for (std::map<std::string, User*>::iterator it = ops.begin(); it != ops.end(); ++it) {
		users.push_back(it->second);
	}
	return users;
}
void Server::who(std::vector<std::string> option, int userFD) {
	(void)userFD;
	#define RPL_WHOREPLY(channel, user, nick, flags, realname)  (":ft.irc 352 " + channel + " " + user + " 42sp.org.br ft.irc " + nick + " " + flags + ":0 " + realname + "\r\n");
	std::string response;

	std::string channelName = option[0];
	Channel* channel = getChannel(channelName);
	std::vector<User *> users = getChannelUsers(channel);
	for (size_t i = 0; i < users.size(); i++) {
		std::string nick = users[i]->getNickName();
		std::string user = users[i]->getRealName();
		std::string realName = ":realname";
		std::string flags = "H ";
		// if ((*it).isOperator(users[i]))
		flags += "@ ";
		response += RPL_WHOREPLY(channelName, user, nick, flags, realName);
	}
	std::cout << "Sending response: " << response << std::endl;
	response += ":ft.irc 315 " + channelName + " :End of /WHO list\r\n";
	std::cout << "Sending response: " << response << std::endl;
	if(send (userFD, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Error sending message" << std::endl;
}