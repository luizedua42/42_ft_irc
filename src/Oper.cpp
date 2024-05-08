/**======================
*            42sp
* @file      : Oper.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 25/04/2024
*========================**/

#include "../include/includes.hpp"

static std::vector<User *> getChannelUsers(Channel* channel) ;

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
<<<<<<< HEAD
	User* client = Server::getUser(userFD);
	std::string requests[] = {"CAP", "USER", "NICK", "JOIN", "PRIVMSG", "QUIT", "MODE", "TOPIC", "INVITE", "KICK", "WHO", "PASS"};

=======
	
	std::string requests[] = {"CAP", "USER", "NICK", "JOIN", "PRIVMSG", "QUIT", "OPER", "MODE", "TOPIC", "INVITE", "KICK", "PART"};
	
>>>>>>> 8268e5b (missing error responses and broadcast of success response)
	do{
		int i = 0;
		std::string option = splittedBuff[0].substr(0, splittedBuff[0].find_first_of(" "));
		std::cout << "buff: " << buff << std::endl;
		for(; i < 12; i++) {
			if(option == requests[i])
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

	Channel* channel = getChannel(channelName);
<<<<<<< HEAD
	if (channel->getModes("i") == true) {
		throw std::runtime_error(ERRMSG_InviteOnly);
=======
	if (channel->getMode() == "inviteOnly") {
		throw std::runtime_error(ERRMSG_INVITEONLY);
>>>>>>> 8268e5b (missing error responses and broadcast of success response)
		return;
    }
	if (!channel->getPassword().empty()) {
        //ask_for password || return a msg asking to join with /join [channelName] [password]
		std::cout << "Password handling goes here";
        return;
    }

	User* user = Server::getUser(userFD);
	channel->addUser(user);
	if (isOperator) {
		channel->promoteToOperator(user->getNickName());
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

	Channel* channel = getChannel(channelName);
	if (channel == NULL && channelName[0] == '#') {
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
	std::map<std::string, User*> users = channel->getNonOperators();
	for (std::map<std::string, User*>::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->second->getNickName() != user->getNickName()) {
			if(it->second->getuserFD() != userFD)
				send(it->second->getuserFD(), response.c_str(), response.size(), 0);
		}
	}
	std::map<std::string, User*> operators = channel->getOperators();
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

void Server::nick(std::vector<std::string> option, int userFD) {
	User* user = Server::getUser(userFD);
	if(user == NULL)
		return;

	std::string nickname = option[0].substr(0, option[0].find('\r'));

	user->setNickName(nickname);
	if(user->getIsAuth() == true) {
		std::string response;
		response = ":ft.irc 001 " + user->getNickName() + " :Welcome to the Internet Relay Chat " + user->getNickName() + "!" + user->getRealName() + "@*\r\n";
		send(userFD, response.c_str(), response.size(), 0);
	}
}

void Server::user(std::vector<std::string> option, int userFD) {
	User* user = Server::getUser(userFD);
	if(user == NULL)
		return;
	std::string username = option[0].substr(0, option[0].find(' '));

	user->setRealName(username);
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
	std::string modeParam = "";
	if (option.size() == 3){
		modeParam = option[2].substr(0, option[2].find('\r'));
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

void Server::topic(std::vector<std::string> option, int clientFd) {
	std::string notopic = " 331 ";
	std::string response;
	
	std::string channelName = option[0];
	
	Channel* channel = getChannel(channelName);
	User* user = Server::getUser(clientFd);
	std::cout << "User: " << user->getNickName() << "userFD: " << clientFd << std::endl;
	if (option.size() == 1) {
		std::cout << "Getting topic of channel: " << channelName << std::endl;
		std::cout << std::boolalpha << channel->getTopic().empty() << std::endl;
		if(channel->getTopic().size() == 0) {
			std::cout << "Topic: " << '"' + channel->getTopic() + '"' << std::endl;
			response = ":ft.irc" + notopic + user->getNickName() + " " + channelName + " :No topic is set.\r\n";
			std::cout << "Sending response: " << response << std::endl;
			//PROBLEM WITH THIS SEND, CLIENT DOES NOT GET THE RESPONSE
			if(send (clientFd, response.c_str(), response.size(), 0) == -1)
				std::cerr << "Error sending message" << std::endl;
			return;
		}
		response = ":ft.irc 332 " + user->getNickName() + " " + channelName + " :" + channel->getTopic() + "\r\n";
		std::cout << "Sending response: " << response << std::endl;
		send(clientFd, response.c_str(), response.size(), 0);
		return;
	} else if (option.size() == 2 ) {
		std::string topic = option[1].substr(1);
		if(channel->getModes("t") == true && !channel->isOperator(user->getNickName())) {
			std::string response = ":ft.irc 482 " + user->getNickName() + " " + channelName + " :You're not channel operator\r\n";
			std::cout << "Sending response: " << response << std::endl;
			send(clientFd, response.c_str(), response.size(), 0);
			return;
		}
		std::cout << "Setting topic of channel: " << channelName << " to: " << topic << std::endl;
		channel->setTopic(topic);
		std::vector<User *> users = getChannelUsers(channel);
		response = ":" + user->getNickName() + "!~" + user->getRealName() + "@ft.irc TOPIC " + channelName + " " + topic + "\r\n";
		for (size_t i = 0; i < users.size(); i++) {
			send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
		}
	} else {
		std::cerr << "Invalid number of arguments" << std::endl;
	}
}

void Server::invite(std::vector<std::string> option, int userFD) {
	(void)userFD;
	std::cout << "Inviting user to channel: " << option[0] << std::endl;
}

void Server::kick(std::vector<std::string> option, int userFD) {
	std::string channelName = option[0];
	std::string userToKick = option[1];
	std::string reason = "test";
	if (option.size() == 3){ 
		std::string reason = option[2].substr(1);
	}
	std::cout << "Kicking user: " << userToKick << " from channel: " << channelName << std::endl;
	Channel* channel = getChannel(channelName);
	User* user = searchUser(userToKick);
	if (user == NULL) {
		std::string response = ":ft.irc 401 " + userToKick + " :No such nick/channel\r\n";
		std::cout << "Sending response: " << response << std::endl;
		send(userFD, response.c_str(), response.size(), 0);
		return;
	}
	User* kicker = Server::getUser(userFD);
	std::vector<User *> users = getChannelUsers(channel);
	std::string response = ":" + kicker->getNickName() + "!" + kicker->getRealName() + "@ft.irc KICK " + channelName + " " + userToKick + " :"+ reason + "\r\n";
	std::cout << "Sending response: " << response << std::endl;
	for (size_t i = 0; i < users.size(); i++) {
		if (users[i]->getNickName() == userToKick) {
			send(users[i]->getuserFD(), response.c_str(), response.size(), 0);
			channel->removeUser(users[i]);
			break;
		}
	}
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
	User& user = getUser(userFD);

    if (option.size() < 2) {
		throw std::runtime_error(ERRMSG_NEEDMOREPARAMS);
    }

    const std::string& channelName = option[0];
    const std::string& userToBeKicked = option[1];

    Channel* channelPtr = getChannel(channelName);
    if (channelPtr == NULL) {
        throw std::runtime_error(ERRMSG_NOSUCHCHANNEL);
    }

    if (!channelPtr->isOperator(user.getNickName())) {
        throw std::runtime_error(ERRMSG_CHANOPRIVSNEEDED);
    }

    channelPtr->removeUser(userToBeKicked);

	std::string response = ":" + user.getNickName() + "!" + user.getRealName() + "KICK " + channelName + userToBeKicked + user.getNickName();
	send(userFD, response.c_str(), response.size(), 0);
}


void Server::part(const std::vector<std::string> option, int userFD) {
	User& user = getUser(userFD);
	std::string response = ":" + user.getNickName() + "!" + user.getRealName() + "PART ";

    if (option[0].empty()) {
		throw std::runtime_error(ERRMSG_NEEDMOREPARAMS);
    }

    std::vector<std::string> channels;
    std::istringstream iss(option[0]);
    std::string channel;
	size_t i = 0;

    while (std::getline(iss, channel, ',')) {
        Channel* channelPtr = getChannel(channel);
        if (channelPtr == NULL) {
			throw std::runtime_error(ERRMSG_NOSUCHCHANNEL); //can we throw this erros without returning to check next group?
        }

        channelPtr->removeUser(user.getNickName());
		if (i != 0)
		{
			response += ",";
		}
		response += channel;
    }

	response +=  "\r\n";
	send(userFD, response.c_str(), response.size(), 0); //broadcast to all
}

