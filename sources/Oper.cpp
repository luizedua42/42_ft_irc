/**======================
*            42sp
* @file      : Oper.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 25/04/2024
*========================**/

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
	void unknownCommand(std::string command, int userFD) {
		std::string response = command + ":Unknown command\r\n";
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
	User* client = Server::getUser(userFD);
	std::string requests[] = {"CAP", "USER", "NICK", "JOIN", "PRIVMSG", "QUIT", "MODE", "TOPIC", "INVITE", "KICK", "WHO", "PASS", "PART"};

	do{
		int i = 0;
		std::string options = splittedBuff[0].substr(0, splittedBuff[0].find_first_of(" "));
		std::cout << "buff: " << buff << std::endl;
		for(; i < 13; i++) {
			if(options == requests[i])
				break;
		}
		if(client->getIsAuth() == false) {
			if (i != 0 && i != 1 && i != 2 && i != 11) {
				std::string response = "You have not registered\r\n";
				std::cout << "Sending response: " << response << std::endl;
				send(userFD, response.c_str(), response.size(), 0);
				return;
			}
		}
		std::string parsedOptions = splittedBuff[0].substr(splittedBuff[0].find_first_of(" ") + 1);
		std::cout << "options: " << options << std::endl;
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
				mode(parseOptions(parsedOptions), userFD);
				break;
			case 7:
				topic(parseOptions(parsedOptions), userFD);
				break;
			case 8:
				invite(parseOptions(parsedOptions), userFD);
				break;
			case 9:
				kick(parseOptions(parsedOptions), userFD);
				break;
			case 10:
				who(parseOptions(parsedOptions), userFD);
				break;
			case 11:
				pass(parseOptions(parsedOptions), userFD);
				break;
			case 12:
				part(parseOptions(parsedOptions), userFD);
				break;
			default:
				mode::unknownCommand(options, userFD);
				break;
		}
		splittedBuff.erase(splittedBuff.begin());
		} while (!splittedBuff.empty());
}

void Server::cap(int userFD) {
	(void)userFD;
	
	std::cout << "CAP" << std::endl;
}

void Server::pass(std::vector<std::string> options, int userFD) {
	User* user = Server::getUser(userFD);
	std::string password = options[0].substr(0, options[0].find('\r'));

	if(user->getPassword().empty())
		user->setPassword(password);

	if (user->getPassword() != Server::getPassword()) {
		std::string response = ":ft.irc 464 :Password incorrect\r\n";
		std::cout << "Sending response: " << response << std::endl;
		send(userFD, response.c_str(), response.size(), 0);
		close(userFD);
		Server::clearUsers(userFD);
		return;
	} else if (user->getPassword() == Server::getPassword()){
		user->setIsAuth(true);
		std::string response = ":ft.irc 001 " + user->getNickName() + " :Welcome to the Internet Relay Chat " + user->getNickName() + "!" + user->getRealName() + "@*\r\n";
		std::cout << "Sending response: " << response << std::endl;
		send(userFD, response.c_str(), response.size(), 0);
	
	}
}

void Server::join(std::vector<std::string> options, int userFD) {
	std::string channelName = options.front().substr(0);
	bool isOperator = false;

	if (!channelExists(channelName)) {
		createChannel(channelName);
		isOperator = true;
	}

	Channel* channelPtr = getChannel(channelName);
	if (channelPtr->getModes("i") == true) {
		// send (ERR_INVITEONLY);
		return;
    }
	if (!channelPtr->getPassword().empty()) {
        //ask_for password || return a msg asking to join with /join [channelName] [password]
		std::cout << "Password handling goes here";
        return;
    }

	User* user = Server::getUser(userFD);
	channelPtr->addUser(user);
	if (isOperator) {
		channelPtr->promoteToOperator(user->getNickName());
	}
	
	std::cout << " Joining channel: " << channelName << std::endl;
	std::string response = ":" + user->getNickName() + " JOIN " + channelName + "\r\n";
	send(userFD, response.c_str(), response.size(), 0);
}

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

void Server::quit(std::vector<std::string> options, int userFD) {
	std::string channel= options[0];
	User* test = Server::getUser(userFD);
	
	std::cout << test->getNickName() <<":" <<"Quittin" << channel << std::endl;
}

void Server::nick(std::vector<std::string> options, int userFD) {
	User* user = Server::getUser(userFD);
	if(user == NULL)
		return;

	std::string nickname = options[0].substr(0, options[0].find('\r'));

	user->setNickName(nickname);
	if(user->getIsAuth() == true) {
		std::string response;
		response = ":ft.irc 001 " + user->getNickName() + " :Welcome to the Internet Relay Chat " + user->getNickName() + "!" + user->getRealName() + "@*\r\n";
		send(userFD, response.c_str(), response.size(), 0);
	}
}

void Server::user(std::vector<std::string> options, int userFD) {
	User* user = Server::getUser(userFD);
	if(user == NULL)
		return;
	std::string username = options[0].substr(0, options[0].find(' '));

	user->setRealName(username);
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
			mode::unknownCommand(mode, clientFd);
			break;
	}
}

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

void Server::invite(const std::vector<std::string> options, int userFD) {
	(void)userFD;
	(void)options;
	// User* user = getUser(userFD);
    // if (options.size() < 2) {
	// 	throw std::runtime_error(ERR_NEEDMOREPARAMS);
    // }

    // const std::string& invitedUser = options[0];
    // const std::string& channelName = options[1];

    // Channel* channelPtr = getChannel(channelName);
    // if (channelPtr == NULL) {
    //     throw std::runtime_error(ERR_NOSUCHCHANNEL);
    // }

    // if (!channelPtr->isUserInChannel(user)) {
	// 	throw std::runtime_error(ERR_NOTONCHANNEL);
    // }

    // std::string response = ":ft.irc" + " 341 " + user.getNickName() + " " + invitedUser + " :" + channelName + "\r\n";
	// send(userFD, response.c_str(), response.size(), 0);
}


void Server::who(std::vector<std::string> options, int userFD) {
	(void)userFD;
	#define RPL_WHOREPLY(channel, user, nick, flags, realname)  (":ft.irc 352 " + channel + " " + user + " 42sp.org.br ft.irc " + nick + " " + flags + ":0 " + realname + "\r\n");
	std::string response;

	std::string channelName = options[0];
	Channel* channelPtr = getChannel(channelName);
	std::vector<User *> users = channelPtr->getAllUsers();
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

void Server::kick(std::vector<std::string> options, int userFD) {
	User* user = getUser(userFD);

    if (options.size() < 2) {
		//response with (ERR_NEEDMOREPARAMS);
		return;
    }

	std::string kickReason;
    const std::string& channelName = options[0];
    const std::string& userToBeKicked = options[1];

	if (options.size() == 3) { 
		kickReason = options[2].substr(1);
	} else {
		kickReason = "no reason";
	}

    Channel* channelPtr = getChannel(channelName);
    if (channelPtr == NULL) {
        //response with (ERR_NOSUCHCHANNEL);
		std::string response = ":ft.irc 403 " + channelName + " :No such channel\r\n";
		// std::cout << "Sending response: " << response << std::endl;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

    if (!channelPtr->isOperator(user->getNickName())) {
        //response with (ERR_CHANOPRIVSNEEDED);
		std::string response = ":ft.irc 482 " + channelName + ":You are not a channel operator\r\n";
		// std::cout << "Sending response: " << response << std::endl;
		send(userFD, response.c_str(), response.size(), 0);
		return;
    }

	std::vector<User *> users = channelPtr->getAllUsers();
	std::string response = ":" + user->getNickName() + "!" + user->getRealName() + "@ft.irc KICK " + channelName + " " + userToBeKicked + " :"+ kickReason + "\r\n";
	std::cout << "Sending response: " << response << std::endl;
	for (size_t i = 0; i < users.size(); i++) {
		send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
	}
    channelPtr->removeUser(userToBeKicked);
}

void Server::part(const std::vector<std::string> options, int userFD) {
	User* user = getUser(userFD);
	std::string partReason = "";

    if (options.empty()) {
		//response with (ERR_NEEDMOREPARAMS);
		return;
    } else if (options.size() > 1) {
		for (size_t i = 1; i < options.size(); i++) {
			partReason += options[i];
		}
	}

    std::istringstream iss(options[0]);
    std::string channelName;

	while (std::getline(iss, channelName, ',')) {
		Channel* channelPtr = getChannel(channelName);
		if (channelPtr == NULL) {
			//response with (ERR_NOSUCHCHANNEL);
			std::string response = ":ft.irc 403 " + channelName + " :No such channel\r\n";
			// std::cout << "Sending response: " << response << std::endl;
			send(userFD, response.c_str(), response.size(), 0);
		} else {
			channelPtr->removeUser(user->getNickName());
			std::string response = ":" + user->getNickName() + "!~" + user->getRealName() + "@* PART " + channelName + " :" + partReason + "\r\n";
			std::vector<User *> users = channelPtr->getAllUsers();
			for (size_t i = 0; i < users.size(); i++) {
				send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
			}
		}
	}
}

