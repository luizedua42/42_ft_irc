/**======================
*            42sp
* @file      : Oper.cpp
* @author    : luizedua
* @email     : luizedua@student.42sp.org.br
* @createdOn : 25/04/2024
*========================**/

#include "../include/includes.hpp"
namespace mode {
	void setOp(std::string channel) {
		(void)channel;
	}
	void delOp(std::string channel) {
		(void)channel;
	}
	void setTopic(std::string channel) {
		(void)channel;
	}
	void delTopic(std::string channel) {
		(void)channel;
	}
	void setInvite(std::string channel) {
		(void)channel;
	}
	void delInvite(std::string channel) {
		(void)channel;
	}
	void setKey(std::string channel) {
		(void)channel;
	}
	void delKey(std::string channel) {
		(void)channel;
	}
	void setLimit(std::string channel) {
		(void)channel;
	}
	void delLimit(std::string channel) {
		(void)channel;
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

void Server::selectOptions(std::string buff, int UserFd) {
	std::vector<std::string> splittedBuff = splitBuff(buff);
	
	std::string requests[] = {"CAP", "USER", "NICK", "JOIN", "PRIVMSG", "QUIT", "OPER", "MODE", "TOPIC", "INVITE", "KICK"};
	
	do{
		int i = 0;
		std::string option = splittedBuff[0].substr(0, splittedBuff[0].find_first_of(" "));
		std::cout << "buff: " << splittedBuff[0] << std::endl;
		for(; i < 10; i++) {
			if(option == requests[i])
				break;
		}

		std::string parsedOptions = splittedBuff[0].substr(splittedBuff[0].find_first_of(" ") + 1);
		std::cout << "option: " << option << std::endl;
		switch (i) {
			case 0:
				cap(UserFd);
				break;
			case 1:
				user(parseOptions(parsedOptions), UserFd);
				break;
			case 2:
				nick(parseOptions(parsedOptions), UserFd);
				break;
			case 3:
				join(parseOptions(parsedOptions), UserFd);
				break;
			case 4:
				privmsg(parseOptions(parsedOptions), UserFd);
				break;
			case 5:
				quit(parseOptions(parsedOptions), UserFd);
				break;
			case 6:
				oper(parseOptions(parsedOptions), UserFd);
				break;
			case 7:
				mode(parseOptions(parsedOptions), UserFd);
				break;
			case 8:
				topic(parseOptions(parsedOptions), UserFd);
				break;
			case 9:
				invite(parseOptions(parsedOptions), UserFd);
				break;
			case 10:
				kick(parseOptions(parsedOptions), UserFd);
				break;
			default:
				std::cerr << "Invalid request: " << option << std::endl;
				break;
		}
		splittedBuff.erase(splittedBuff.begin());
		} while (!splittedBuff.empty());
		// buff.clear();
}

void Server::cap(int UserFd) {
	(void)UserFd;
	
	std::cout << "CAP" << std::endl;
	// send(UserFd, "\r\n", 2, 0);
}

void Server::join(std::vector<std::string> options, int UserFd) {
	std::string channelName = options.front().substr(1);
	bool isOperator = false;

	if (!channelExists(channelName)) {
		createChannel(channelName);
		isOperator = true;
	}

	Channel* channel = getChannel(channelName);
	if (channel->getMode() == "inviteOnly") {
		throw std::runtime_error(ERRMSG_InviteOnly);
		return;
    }
	if (!channel->getPassword().empty()) {
        //ask_for password || return a msg asking to join with /join [channelName] [password]
		std::cout << "Password handling goes here";
        return;
    }

	User& user = Server::getUser(UserFd);
	channel->addUser(&user);
	if (isOperator) {
		channel->promoteToOperator(user.getNickName());
	}
	
	std::cout << " Joining channel: " << channelName << std::endl;
	std::string response = ":" + user.getNickName() + " JOIN " + channelName + "\r\n";
	send(UserFd, response.c_str(), response.size(), 0);
}

void Server::privmsg(std::vector<std::string> options, int UserFd) {
	std::cout << "Sending message to channel: " << options[0] << " - " << std::endl;\
	(void)UserFd;

}

void Server::quit(std::vector<std::string> options, int UserFd) {
	std::string channel= options[0];
	User& test = Server::getUser(UserFd);
	
	std::cout << test.getNickName() <<":" <<"Quittin" << channel << std::endl;
}

void Server::nick(std::vector<std::string> option, int UserFd) {
	User& user = Server::getUser(UserFd);

	std::string nickname = option[0].substr(0, option[0].find('\r'));

	user.setNickName(nickname);

	std::string response;
	response = ":ft.irc 001 " + user.getNickName() + " :Welcome to the Internet Relay Chat " + user.getNickName() + "!" + user.getRealName() + "@*\r\n";
	send(UserFd, response.c_str(), response.size(), 0);
}

void Server::user(std::vector<std::string> option, int UserFd) {
	User& user = Server::getUser(UserFd);

	std::string username = option[0].substr(0, option[0].find(' '));

	user.setRealName(username);
}


void Server::oper(std::vector<std::string> option, int UserFd) {
	(void)UserFd;
	std::cout << "Opering user: " << option[0] << std::endl;
}

void Server::mode(std::vector<std::string> option, int clientFd) {
	if(option.size() < 3) {
		std::cerr << "Invalid number of arguments" << std::endl;
		return;
	}
	(void)clientFd;
	int i = 0;
	std::string modes[] = {"-i", "+i", "-t", "+t", "-k", "+k", "-o", "+o", "-l", "+l"};
	std::string channel = option[0];
	std::string mode = option[1].substr(0, option[1].find('\r'));
	std::string user = option[2].substr(0, option[2].find('\r'));
	std::cout << "Setting mode: " << mode << " in channel: " << channel << " to user: " << user << std::endl;
	for(; i < 10; i++) {
		if(mode == modes[i])
			break;
	}
	switch (i) {
		case 0:
			mode::delInvite(channel);
			break;
		case 1:
			mode::setInvite(channel);
			break;
		case 2:
			mode::delTopic(channel);
			break;
		case 3:
			mode::setTopic(channel);
			break;
		case 4:
			mode::delKey(channel);
			break;
		case 5:
			mode::setKey(channel);
			break;
		case 6:
			mode::delOp(channel);
			break;
		case 7:
			mode::setOp(channel);
			break;
		case 8:
			mode::delLimit(channel);
			break;
		case 9:
			mode::setLimit(channel);
			break;
		default:
			std::cerr << "Invalid mode" << std::endl;
			break;
	}
}

void Server::topic(std::vector<std::string> option, int clientFd) {
	(void)clientFd;
	std::string channel = option[0];
	std::string topic = option[1];
	if (option.size() == 1) {
		std::cout << "Getting topic of channel: " << channel << std::endl;
	} else if (option.size() == 2) {
		std::cout << "Setting topic of channel: " << channel << " to: " << topic << std::endl;
	} else {
		std::cerr << "Invalid number of arguments" << std::endl;
	}
}

void Server::invite(std::vector<std::string> option, int UserFd) {
	(void)UserFd;
	std::cout << "Inviting user to channel: " << option[0] << std::endl;
}

void Server::kick(std::vector<std::string> option, int UserFd) {
	(void)option;
	(void)UserFd;
//	std::string channel = option[0].substr(0, option[0].find('\r'));
//	std::string user = option[1];
// std::cout << "Kicking " << user << " from channel: " << channel << std::endl;
}